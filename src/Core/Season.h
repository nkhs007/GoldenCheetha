/*
 * Copyright (c) 2009 Justin F. Knotzke (jknotzke@shampoo.ca)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef SEASON_H_
#define SEASON_H_
#include "GoldenCheetah.h"

#include <QString>
#include <QDate>
#include <QDialog>
#include <QFile>
#include <QApplication>
#include <QTreeWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>

#include "Context.h"

class Phase;

class SeasonEvent
{
    Q_DECLARE_TR_FUNCTIONS(Season)

    public:
        static QStringList priorityList();

        SeasonEvent(QString name, QDate date, int priority=0, QString description="", QString id="") : name(name), date(date), priority(priority), description(description), id(id) {}

        QString name;
        QDate date;
        int priority;
        QString description;
        QString id; // unique id
};

class Season
{
    Q_DECLARE_TR_FUNCTIONS(Season)

    public:
        static QList<QString> types;
        enum SeasonType { season=0, cycle=1, adhoc=2, temporary=3 };
        //typedef enum seasontype SeasonType;

        Season();

        QDate getStart();
        QDate getEnd();
        int getSeed() { return _seed; }
        int getLow() { return _low; }
        int getMaxRamp() { return _ramp; }
        QString getName();
        int days() { return _days; } // how many days in the season, -1 if never ending
        int prior(); // if a relative season, how many days prior does it cover e.g. Last 21 days returns 21. 0 if not relative
        int getType();
        static bool LessThanForStarts(const Season &a, const Season &b);

        void setStart(QDate _start);
        void setEnd(QDate _end);
        void setName(QString _name);
        void setType(int _type);
        void setSeed(int x) { _seed = x; }
        void setLow(int x) { _low = x; }
        void setMaxRamp(int x) { _ramp = x; }
        QUuid id() const { return _id; }
        void setId(QUuid x) { _id = x; }
        QVector<int> &load() { return _load; }

        int type;
        QDate start; // first day of the season
        QDate end; // last day of the season

        QString name; // name, typically users name them by year e.g. "2011 Season"

        QVector<int> _load; // array of daily planned load

        QList<Phase> phases;
        QList<SeasonEvent> events;

    protected:

        int _days; // how many days in this season?
        int _seed;
        int _low; // low point for SB .. default to -50
        int _ramp; // max ramp rate for CTL we want to see
        QUuid _id; // unique id

};

class EditSeasonDialog : public QDialog
{
    Q_OBJECT
    G_OBJECT


    public:
        EditSeasonDialog(Context *, Season *);

    public slots:
        void applyClicked();
        void cancelClicked();
        void nameChanged();

    private:
        Context *context;
        Season *season;

        QPushButton *applyButton, *cancelButton;
        QLineEdit *nameEdit;
        QComboBox *typeEdit;
        QDateEdit *fromEdit, *toEdit;
        QDoubleSpinBox *seedEdit;
        QDoubleSpinBox *lowEdit;
};

class EditSeasonEventDialog : public QDialog
{
    Q_OBJECT
    G_OBJECT


    public:
        EditSeasonEventDialog(Context *, SeasonEvent *, Season &);

    public slots:
        void applyClicked();
        void cancelClicked();
        void nameChanged();

    private:
        Context *context;
        SeasonEvent *event;

        QPushButton *applyButton, *cancelButton;
        QLineEdit *nameEdit;
        QDateEdit *dateEdit;
        QComboBox *priorityEdit;
        QTextEdit *descriptionEdit;
};

class Seasons : public QObject {

    Q_OBJECT;

    public:
        Seasons(QDir home) : home(home) { readSeasons(); }
        Seasons() {
            Season season;
            QDate today = QDate::currentDate();
            QDate eom = QDate(today.year(), today.month(), today.daysInMonth());

            // add Default Date Ranges
            season.setName(tr("All Dates"));
            season.setType(Season::temporary);
            season.setStart(QDate::currentDate().addYears(-50));
            season.setEnd(QDate::currentDate().addYears(50));
            season.setId(QUuid("{00000000-0000-0000-0000-000000000001}"));
            seasons.append(season);

            season.setName(tr("This Year"));
            season.setType(Season::temporary);
            season.setStart(QDate(today.year(), 1,1));
            season.setEnd(QDate(today.year(), 12, 31));
            season.setId(QUuid("{00000000-0000-0000-0000-000000000002}"));
            seasons.append(season);

            season.setName(tr("This Month"));
            season.setType(Season::temporary);
            season.setStart(QDate(today.year(), today.month(),1));
            season.setEnd(eom);
            season.setId(QUuid("{00000000-0000-0000-0000-000000000003}"));
            seasons.append(season);

            season.setName(tr("Last Month"));
            season.setType(Season::temporary);
            season.setStart(QDate(today.year(), today.month(),1).addMonths(-1));
            season.setEnd(QDate(today.year(), today.month(),1).addDays(-1));
            season.setId(QUuid("{00000000-0000-0000-0000-000000000013}"));
            seasons.append(season);

            season.setName(tr("This Week"));
            season.setType(Season::temporary);
            // from Mon-Sun
            QDate wstart = QDate::currentDate();
            wstart = wstart.addDays(Qt::Monday - wstart.dayOfWeek());
            QDate wend = wstart.addDays(6); // first day + 6 more
            season.setStart(wstart);
            season.setEnd(wend);
            season.setId(QUuid("{00000000-0000-0000-0000-000000000004}"));
            seasons.append(season);

            season.setName(tr("Last Week"));
            season.setType(Season::temporary);
            // from Mon-Sun
            season.setStart(wstart.addDays(-7));
            season.setEnd(wend.addDays(-7));
            season.setId(QUuid("{00000000-0000-0000-0000-000000000014}"));
            seasons.append(season);

            season.setName(tr("Last 24 hours"));
            season.setType(Season::temporary);
            season.setStart(today.addDays(-1)); // today plus previous 6
            season.setEnd(today);
            season.setId(QUuid("{00000000-0000-0000-0000-000000000016}"));
            seasons.append(season);

            season.setName(tr("Last 7 days"));
            season.setType(Season::temporary);
            season.setStart(today.addDays(-6)); // today plus previous 6
            season.setEnd(today);
            season.setId(QUuid("{00000000-0000-0000-0000-000000000005}"));
            seasons.append(season);

            season.setName(tr("Last 14 days"));
            season.setType(Season::temporary);
            season.setStart(today.addDays(-13));
            season.setEnd(today);
            season.setId(QUuid("{00000000-0000-0000-0000-000000000006}"));
            seasons.append(season);

            season.setName(tr("Last 21 days"));
            season.setType(Season::temporary);
            season.setStart(today.addDays(-20));
            season.setEnd(today);
            season.setId(QUuid("{00000000-0000-0000-0000-000000000011}"));
            seasons.append(season);

            season.setName(tr("Last 28 days"));
            season.setType(Season::temporary);
            season.setStart(today.addDays(-27));
            season.setEnd(today);
            season.setId(QUuid("{00000000-0000-0000-0000-000000000007}"));
            seasons.append(season);

            season.setName(tr("Last 6 weeks"));
            season.setType(Season::temporary);
            season.setEnd(today);
            season.setStart(today.addDays(-41));
            season.setId(QUuid("{00000000-0000-0000-0000-000000000015}"));
            seasons.append(season);

            season.setName(tr("Last 2 months"));
            season.setType(Season::temporary);
            season.setEnd(today);
            season.setStart(today.addMonths(-2));
            season.setId(QUuid("{00000000-0000-0000-0000-000000000008}"));
            seasons.append(season);

            season.setName(tr("Last 3 months"));
            season.setType(Season::temporary);
            season.setEnd(today);
            season.setStart(today.addMonths(-3));
            season.setId(QUuid("{00000000-0000-0000-0000-000000000012}"));
            seasons.append(season);

            season.setName(tr("Last 6 months"));
            season.setType(Season::temporary);
            season.setEnd(today);
            season.setStart(today.addMonths(-6));
            season.setId(QUuid("{00000000-0000-0000-0000-000000000009}"));
            seasons.append(season);

            season.setName(tr("Last 12 months"));
            season.setType(Season::temporary);
            season.setEnd(today);
            season.setStart(today.addMonths(-12));
            season.setId(QUuid("{00000000-0000-0000-0000-000000000010}"));
            seasons.append(season);

        }
        void readSeasons();
        int newSeason(QString name, QDate start, QDate end, int type);
        void updateSeason(int index, QString name, QDate start, QDate end, int type);
        void deleteSeason(int);
        void writeSeasons();
        QList<Season> seasons;

        // get first season the date falls within
        Season seasonFor(QDate);

    signals:
        void seasonsChanged();


    private:
        QDir home;
};

class SeasonTreeView : public QTreeWidget
{
    Q_OBJECT

    public:
        SeasonTreeView(Context *);

        // for drag/drop
        QStringList mimeTypes () const;
        QMimeData * mimeData ( const QList<QTreeWidgetItem *> items ) const;

    signals:
        void itemMoved(QTreeWidgetItem* item, int previous, int actual);

    protected:
        void dropEvent(QDropEvent* event);
        Context *context;


};

class Phase : public Season
{
    Q_DECLARE_TR_FUNCTIONS(Phase)

    public:
        static QList<QString> types;
        enum PhaseType { phase=100, prep=101, base=102, build=103, peak=104, camp=120 };

        Phase();
        Phase(QString _name, QDate _start, QDate _end);

};

class EditPhaseDialog : public QDialog
{
    Q_OBJECT
    G_OBJECT


    public:
        EditPhaseDialog(Context *, Phase *, Season &);

    public slots:
        void applyClicked();
        void cancelClicked();
        void nameChanged();

    private:
        Context *context;
        Phase *phase;

        QPushButton *applyButton, *cancelButton;
        QLineEdit *nameEdit;
        QComboBox *typeEdit;
        QDateEdit *fromEdit, *toEdit;
        QDoubleSpinBox *seedEdit;
        QDoubleSpinBox *lowEdit;
};

class SeasonEventTreeView : public QTreeWidget
{
    Q_OBJECT

    public:
        SeasonEventTreeView();

    signals:
        void itemMoved(QTreeWidgetItem* item, int previous, int actual);

    protected:
        void dropEvent(QDropEvent* event);
};
#endif /* SEASON_H_ */
