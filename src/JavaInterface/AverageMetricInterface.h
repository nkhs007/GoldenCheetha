#ifndef AverageMetriceInterface_h
#define AverageMetriceInterface_h

#endif // AverageMetriceInterface_h
#include <QVector>
#include <QDebug>
#include <jni.h>
#include "RideFile.h"
#include "Zones.h"
#include "HrZones.h"
#include "PaceZones.h"
#include "RideItem.h"
#include "Context.h"
#include "PMCData.h"
#include "RideMetric.h"

class AverageMetrice : public QObject{

public:
    AverageMetrice() { RideMetricFactory::instance().initialize(); }
    QStringList requiredMatrics;
    typedef struct _MatricValue {
        jclass cls;
        jmethodID constructortorID;
        jmethodID setKey;
        jmethodID setValue;
    }_MatricValue;

    _MatricValue * jniMatricValueStructure = NULL;

    double athleteWeight;
    double athleteHeight;
    bool isMetric;

private:
    bool isAverage;

public:
    //Initialising structure
    void _intializeMatricValueStructure(JNIEnv *env);
    void jniAquireRidePoints(JNIEnv *env,jobject gen,RideFile *rideFile);
    void jniAquireAthleteData(JNIEnv *env,jobject gen);
    void jniAquireMatricsToCalculate(JNIEnv *env,jobject gen);
    //Zone data
    void jniAquirePowerZone(JNIEnv *env,jobject gen, Zones *zones[2]);
    void jniAquireHeartZone(JNIEnv *env, jobject gen, HrZones *hrzones[2]);
    void jniAquirePaceZone(JNIEnv *env,jobject gen, PaceZones *pacezones[2]);
    //calculation
    void calculateOtherMetrics(JNIEnv *env,Athlete *athlete, jobject gen);
    void setCalculatedValuesToRideItem(RideItem *rideItem, QHash<QString, RideMetricPtr> notmetrics);
    void calculateOtherMetricAverage(JNIEnv *env, jobject gen, Athlete *athlete);

    void jniAquireRideFileData(JNIEnv *env, jobject gen, Athlete *athlete);
    //Calculating Average vales of all metrices
    void calculateAverageValuesOfAllMetrices(JNIEnv *env, jobject gen, Athlete *athlete);
    //Preparing data for PMC graph
    void createPmcGraphData(JNIEnv *env, jobject gen, Athlete *athlete);

};


