#include "com_svlabs_svt_matrics_SaarMatricsGenerator.h"
#include "RideFile.h"
#include "RideItem.h"
#include "matricsinterface.h"
#include "Context.h"
#include "Athlete.h"
#include "MainWindow.h"


void Java_com_svlabs_svt_matrics_SaarMatricsGenerator_analyseMatric(JNIEnv *env, jobject gen)
{
    jniGetMatricsToCalculate(env,gen);
    /*foreach (const QString &str, testMatrics) {
       qDebug() << str;
    }*/
    RideFile file;

    jniConvertRidePoints(env,gen,&file);

    int* firstarg = new int(0);
    QApplication *qApplication = new QApplication(*firstarg, NULL);


    Context *context = new Context();

    Athlete *athlete = new Athlete(context);
    athlete->setAtheletWeight(weight);
    athlete->setAthleteHeight(height);


    RideItem *rideItem = new RideItem(&file, context);

    rideItem->isRun = file.isRun();
    rideItem->isSwim = file.isSwim();

    rideItem->ride()->context = context;

    QHash<QString, RideMetricPtr> notmetrics = RideMetric::computeMetrics(rideItem, Specification(),testMatrics);
    qDebug() << "notmetrics = " << notmetrics;
    qDebug() << "notmetrics.size = " << notmetrics.size();
    _initMatricValueStructure(env);
    qDebug() << "Matric value array created";

    QHashIterator<QString, RideMetricPtr> i(notmetrics);
    while (i.hasNext()) {
        i.next();
        RideMetricPtr rideMatPtr = i.value();
        QString key = i.key();
        qDebug() << "Key : " << key << ", RideMatValue : " << rideMatPtr->value();
        if( env == NULL ) {
            qDebug() << "env is null" ;
        } else {
            qDebug() << "env is not null" ;
        }
        if( jniMatricValueStruct == NULL ) {
            qDebug() << "jniMatricValueStruct is null" ;
        } else {
            qDebug() << "jniMatricValueStruct is not null" ;
        }
        if( jniMatricValueStruct->cls == NULL ) {
            qDebug() << "jniMatricValueStruct->cls is null" ;
        } else {
            qDebug() << "jniMatricValueStruct->cls is not null" ;
        }
        if( jniMatricValueStruct->constructortorID == NULL ) {
            qDebug() << "jniMatricValueStruct->constructortorID is null" ;
        } else {
            qDebug() << "jniMatricValueStruct->constructortorID is not null" ;
        }
        jobject jniMatricValue = env->NewObject(jniMatricValueStruct->cls, jniMatricValueStruct->constructortorID);
        if( jniMatricValue == NULL ) {
            qDebug() << "Not able to create jni matric value" << "Key : " << key << ", RideMatValue : " << rideMatPtr->value();
        } else {
            qDebug() << "jniMatricValueStruct->constructortorID is created" ;
        }
        env->CallObjectMethod(jniMatricValue,jniMatricValueStruct->setKey, env->NewStringUTF(key.toUtf8().constData()));
        env->CallObjectMethod(jniMatricValue,jniMatricValueStruct->setValue, rideMatPtr->value());
        jmethodID jniAddRidePointMethod = env->GetMethodID(env->GetObjectClass(gen),"addMatricValue","(Lcom/svlabs/svt/ride/MatricValue;)V");
        env->CallObjectMethod(gen, jniAddRidePointMethod, jniMatricValue);
        env->DeleteGlobalRef(jniMatricValue);
        env->DeleteLocalRef(jniMatricValue);
    }
    qDebug() << "Returing from Java_com_svlabs_svt_matrics_SaarMatricsGenerator_analyseMatric";
}
