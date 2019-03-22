#include "com_svlabs_svt_matrics_SaarMatricsGenerator.h"
#include "RideFile.h"
#include "RideItem.h"
#include "matricsinterface.h"
#include "Context.h"
#include "Athlete.h"
#include "MainWindow.h"
#include "Zones.h"
#include "HrZones.h"
#include "PaceZones.h"


void Java_com_svlabs_svt_matrics_SaarMatricsGenerator_analyseMatric(JNIEnv *env, jobject gen)
{
    jniGetMatricsToCalculate(env,gen);
    RideFile file;

    jniConvertRidePoints(env,gen,&file);
//    jniGetAthleteData(env,gen,&file);

    Zones *zones[2];
    HrZones *hrzones[2];
    PaceZones *pacezones[2];

     jniConvertPowerZone(env, gen, zones);
     jniConvertHeartZone(env, gen, hrzones);
     jniConvertPaceZone(env, gen, pacezones);

    int* firstarg = new int(0);
    QApplication *qApplication = new QApplication(*firstarg, NULL);

    Context *context = new Context();

    Athlete *athlete = new Athlete(context, zones, hrzones, pacezones);
    athlete->setAtheletWeight(weight);
    athlete->setAthleteHeight(height);

    RideItem *rideItem = new RideItem(&file, context);

    rideItem->isRun = file.isRun();
    rideItem->isSwim = file.isSwim();
    rideItem->setStartTime(file.startTime());
    rideItem->planned = false;

    rideItem->zoneRange = context->athlete->zones(false)->whichRange(file.startTime().date());
    rideItem->hrZoneRange = context->athlete->hrZones(false)->whichRange(file.startTime().date());
    rideItem->paceZoneRange = context->athlete->paceZones(false)->whichRange(file.startTime().date());

    rideItem->ride()->context = context;
    rideItem->weight = weight;

    athlete->rideCache->rides().append(rideItem);

    QHash<QString, RideMetricPtr> notmetrics = RideMetric::computeMetrics(rideItem, Specification(),testMatrics);
    qDebug() << "notmetrics = " << notmetrics;
    _initMatricValueStructure(env);
    qDebug() << "Matric value array created";

     calculateMetrics(env, context, gen, rideItem, notmetrics);
    QHashIterator<QString, RideMetricPtr> i(notmetrics);
    while (i.hasNext()) {
        i.next();
        RideMetricPtr rideMatPtr = i.value();
        QString key = i.key();
        qDebug() << "Key : " << key << ", RideMatValue : " << rideMatPtr->value();
        jobject jniMatricValue = env->NewObject(jniMatricValueStruct->cls, jniMatricValueStruct->constructortorID);
        if( jniMatricValue == NULL ) {
            qDebug() << "Not able to create jni matric value" << "Key : " << key << ", RideMatValue : " << rideMatPtr->value();
        }
        env->CallObjectMethod(jniMatricValue,jniMatricValueStruct->setKey, env->NewStringUTF(key.toUtf8().constData()));
        env->CallObjectMethod(jniMatricValue,jniMatricValueStruct->setValue, rideMatPtr->value());
        jmethodID jniAddRidePointMethod = env->GetMethodID(env->GetObjectClass(gen),"addMatricValue","(Lcom/svlabs/svt/ride/MatricValue;)V");
        env->CallObjectMethod(gen, jniAddRidePointMethod, jniMatricValue);
    }
    qDebug() << "Returing from Java_com_svlabs_svt_matrics_SaarMatricsGenerator_analyseMatric";
}
