#include "com_svlabs_svt_matrics_SaarMatricsGenerator.h"
#include "RideFile.h"
#include "RideItem.h"
#include "matricsinterface.h"


void Java_com_svlabs_svt_matrics_SaarMatricsGenerator_analyseMatric(JNIEnv *env, jobject gen)
{
    jniGetMatricsToCalculate(env,gen);
    /*foreach (const QString &str, testMatrics) {
       qDebug() << str;
    }*/
    RideFile file;

    jniConvertRidePoints(env,gen,&file);

    RideItem *rideItem = new RideItem(&file, NULL);
    rideItem->isRun = file.isRun();
    rideItem->isSwim = file.isSwim();
    QHash<QString,RideMetricPtr> notmetrics = RideMetric::computeMetrics(rideItem, Specification(),testMatrics);
    _initMatricValueStructure(env);
    //jobjectArray jniMatricValueArray = env->NewObjectArray(notmetrics.size(),jniMatricValueStruct->cls, NULL);
    qDebug() << "Matric value array created";
    QHashIterator<QString, RideMetricPtr> i(notmetrics);
    while (i.hasNext()) {
        i.next();
        RideMetricPtr rideMatPtr = i.value();
        QString key = i.key();
        qDebug() << "Key : "+key+" RideMatValue: "<<rideMatPtr->value()<<rideMatPtr->units(true);
        jobject jniMatricValue = env->NewObject(jniMatricValueStruct->cls, jniMatricValueStruct->constructortorID);
        qDebug() << "Created a matric value";
        env->CallObjectMethod(jniMatricValue,jniMatricValueStruct->setKey,env->NewStringUTF(key.toUtf8().constData()));
        env->CallObjectMethod(jniMatricValue,jniMatricValueStruct->setValue,rideMatPtr->value());
        qDebug() << "sets the values also";
        jmethodID jniAddRidePointMethod = env->GetMethodID(env->GetObjectClass(gen),"addMatricValue","(Lcom/svlabs/svt/ride/MatricValue;)V");
        qDebug() << "Got the method addMatricValue";
        env->CallObjectMethod(gen,jniAddRidePointMethod,jniMatricValue);
        qDebug() << "Called addMatricValue";
        //env->SetObjectArrayElement(jniMatricValueArray, count++,jniMatricValue );
    }
}
