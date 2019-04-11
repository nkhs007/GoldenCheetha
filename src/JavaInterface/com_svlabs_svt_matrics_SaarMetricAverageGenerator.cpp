#include "RideFile.h"
#include "RideItem.h"
#include "AverageMetricInterface.h"
#include "Context.h"
#include "Athlete.h"
#include "MainWindow.h"
#include "Zones.h"
#include "HrZones.h"
#include "PaceZones.h"
#include "JavaInterface/com_svlabs_svt_matrics_SaarMetricAverageGenerator.h"

void Java_com_svlabs_svt_matrics_SaarMetricAverageGenerator_analyseMatric(JNIEnv *env, jobject gen){

    int* firstarg = new int(0);
    Zones *zones[2];
    HrZones *hrzones[2];
    PaceZones *pacezones[2];

    QApplication *qApplication = new QApplication(*firstarg, NULL);

    AverageMetrice *avg = new AverageMetrice();
    avg->_intializeMatricValueStructure(env);
    avg->jniAquirePowerZone(env, gen, zones);
    avg->jniAquireHeartZone(env, gen, hrzones);
    avg->jniAquirePaceZone(env, gen, pacezones);
    avg->jniAquireMatricsToCalculate(env,gen);
    avg->jniAquireAthleteData(env,gen);

    Context *context = new Context();
    Athlete *athlete = new Athlete(context, zones, hrzones, pacezones);
    athlete->setAtheletWeight(avg->athleteWeight);
    athlete->setAthleteHeight(avg->athleteHeight);

    avg->jniAquireRideFileData(env,gen,athlete);
    avg->calculateAverageValuesOfAllMetrices(env,gen,athlete);

    qApplication->exit(0);
    qApplication->quit();
    qDebug() << "qApplication->exit(0)";
    delete context;
    delete firstarg;
    delete avg->jniMatricValueStructure;
    delete avg;
    free(athlete);
//    free(qApplication);
    qDebug() << "Exiting Metric Calculation analysis";
}
