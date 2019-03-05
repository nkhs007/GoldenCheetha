#ifndef RIDESINTERFACE_H
#define RIDESINTERFACE_H

#endif // RIDESINTERFACE_H
#include "jni.h"

#include <QDebug>
typedef struct _RideFilePoint {
    jclass cls;
    jmethodID constructortorID;
    jfieldID hr;
    jfieldID km;
    jfieldID lon;
    jfieldID lat;
    jfieldID secs;
    jfieldID cad;
    jfieldID rcad;
    jfieldID kph;
    jfieldID watts;
    jfieldID alt;
    jfieldID temp;
    jfieldID slope;
}_RideFilePoint;

typedef struct _svtRideFile {
    jclass cls;
    jmethodID constructorId;
    jfieldID starttime;
    jfieldID recintsecs;
    jfieldID tags;
    jfieldID samples;
}_svtRideFile;

_svtRideFile *jniRideFileStruct = NULL;
_RideFilePoint *jniRidePointStruct = NULL;


void createSVTRideFileJavaObject(JNIEnv *env) {
    qDebug() << "RIDESINTERFACE_H : createSVTRideFileJavaObject : Creating the RideFile";
    jniRideFileStruct = new _svtRideFile;

    jniRideFileStruct->cls = env->FindClass("com/svlabs/svt/ride/RideFile");
    if(jniRideFileStruct->cls != NULL) {
        qDebug() <<"RIDESINTERFACE_H : createSVTRideFileJavaObject : Sucessfully created com/svlabs/svt/ride/RideFile";
    }

    jniRideFileStruct->constructorId = env->GetMethodID(jniRideFileStruct->cls, "<init>", "()V");
    if(jniRideFileStruct->constructorId != NULL){
        qDebug() <<"RIDESINTERFACE_H : createSVTRideFileJavaObject : Sucessfully created RideFile constructor   ";
    }

    jniRideFileStruct->starttime = env->GetFieldID(jniRideFileStruct->cls, "starttime", "J");
    if(jniRideFileStruct->starttime != NULL){
        qDebug() <<"RIDESINTERFACE_H : createSVTRideFileJavaObject : Sucessfully created RideFile starttime   ";
    }

    jniRideFileStruct->recintsecs = env->GetFieldID(jniRideFileStruct->cls, "recintsecs", "D");
    if(jniRideFileStruct->recintsecs != NULL){
       qDebug() <<"RIDESINTERFACE_H : createSVTRideFileJavaObject : Sucessfully created RideFile recintsecs   ";
    }

    jniRideFileStruct->tags = env->GetFieldID(jniRideFileStruct->cls, "tags", "Ljava/util/HashMap;");
    if(jniRideFileStruct->tags != NULL){
        qDebug() <<"RIDESINTERFACE_H : createSVTRideFileJavaObject : Sucessfully created RideFile tags   ";
    }

    jniRideFileStruct->samples = env->GetFieldID(jniRideFileStruct->cls, "samples", "[Lcom/svlabs/svt/ride/RideFilePoint;");
    if(jniRideFileStruct->samples != NULL){
        qDebug() <<"RIDESINTERFACE_H : createSVTRideFileJavaObject : Sucessfully created RideFile samples   ";
    }

    qDebug() <<"RIDESINTERFACE_H : createSVTRideFileJavaObject : Successfully created RideFile";
}

void createRideFilePointJavaObject(JNIEnv *env){
    qDebug() << "Creating the RideFilePoint";
    jniRidePointStruct = new _RideFilePoint;
    jniRidePointStruct->cls = env->FindClass("com/svlabs/svt/ride/RideFilePoint");
    if(jniRidePointStruct->cls != NULL)
        qDebug() <<"sucessfully created com/svlabs/svt/ride/RideFilePoint";
    jniRidePointStruct->constructortorID = env->GetMethodID(jniRidePointStruct->cls, "<init>", "()V");
    if(jniRidePointStruct->constructortorID != NULL){
        qDebug() << "sucessfully created RideFilePoint constructor   ";
    }
    jniRidePointStruct->hr = env->GetFieldID(jniRidePointStruct->cls, "hr", "D");
    jniRidePointStruct->km = env->GetFieldID(jniRidePointStruct->cls, "km", "D");
    jniRidePointStruct->lon = env->GetFieldID(jniRidePointStruct->cls, "lon", "D");
    jniRidePointStruct->lat = env->GetFieldID(jniRidePointStruct->cls, "lat", "D");
    jniRidePointStruct->secs = env->GetFieldID(jniRidePointStruct->cls, "secs", "D");
    jniRidePointStruct->cad = env->GetFieldID(jniRidePointStruct->cls, "cad", "D");
    jniRidePointStruct->rcad = env->GetFieldID(jniRidePointStruct->cls, "rcad", "D");
    jniRidePointStruct->kph = env->GetFieldID(jniRidePointStruct->cls, "kph", "D");
    jniRidePointStruct->watts = env->GetFieldID(jniRidePointStruct->cls, "watts", "D");
    jniRidePointStruct->alt = env->GetFieldID(jniRidePointStruct->cls, "alt", "D");
    jniRidePointStruct->temp = env->GetFieldID(jniRidePointStruct->cls, "temp", "D");
    jniRidePointStruct->slope = env->GetFieldID(jniRidePointStruct->cls, "slope", "D");
    qDebug() << "Successfully created RideFilePoint";
}

