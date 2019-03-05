#ifndef MATRICSINTERFACE_H
#define MATRICSINTERFACE_H

#endif // MATRICSINTERFACE_H
#include <QVector>
#include <QDebug>
#include <jni.h>
#include "RideFile.h"
QStringList testMatrics;
typedef struct _MatricValue {
    jclass cls;
    jmethodID constructortorID;
    jmethodID setKey;
    jmethodID setValue;
}_MatricValue;

_MatricValue * jniMatricValueStruct = NULL;

void _initMatricValueStructure(JNIEnv *env){
    if(NULL == jniMatricValueStruct){
        jniMatricValueStruct = new _MatricValue;
        jniMatricValueStruct->cls = env->FindClass("com/svlabs/svt/ride/MatricValue");
        if(jniMatricValueStruct->cls != NULL)
            qDebug() <<"sucessfully created com/svlabs/svt/ride/MatricValue";
        jniMatricValueStruct->constructortorID = env->GetMethodID(jniMatricValueStruct->cls, "<init>", "()V");
        if(jniMatricValueStruct->constructortorID != NULL){
            qDebug() << "sucessfully created MatricValue constructor   ";
        }
        jniMatricValueStruct->setKey = env->GetMethodID(jniMatricValueStruct->cls,"setKey","(Ljava/lang/String;)V");
        jniMatricValueStruct->setValue = env->GetMethodID(jniMatricValueStruct->cls,"setValue","(D)V");
    }
}
void jniConvertRidePoints(JNIEnv *env,jobject gen,RideFile *rideFile){

    qDebug() << "matricsinterface.h : jniConvertRidePoints : Start converting";
    //com.svlabs.svt.matrics.SaarMatricsGenerator
    jclass genrator=env->GetObjectClass(gen);
    if( genrator != NULL){
        qDebug() << "Got com.svlabs.svt.matrics.SaarMatricsGenerator Object";
    }else{
         qDebug() << "Problem while gettig com.svlabs.svt.matrics.SaarMatricsGenerator Object";
    }
    jmethodID methodId=env->GetMethodID(genrator, "getRidePoints", "()[Lcom/svlabs/svt/ride/RideFilePoint;");
    if( methodId != NULL){
        qDebug() << "Got getRidePoints Method";
    }else {
         qDebug() << "Not able to get getRidePoints method";
    }
    jobjectArray pointsToCal = (jobjectArray)env->CallObjectMethod(gen, methodId);
    if(pointsToCal != NULL){
        qDebug() << "call to getRidePoints done";
    }else{
        qDebug() << "Not able call the method getRidePoints";
    }
    jsize length = env->GetArrayLength(pointsToCal);
    qDebug() << "Points to analysis : " << length;
    for (int i=0;i<length;i++) {
        jobject jRidePoint = (jstring) (env->GetObjectArrayElement(pointsToCal, i));
        jclass jRidePointClass = env->GetObjectClass(jRidePoint);
        RideFilePoint point;

        jfieldID jhrFeild = env->GetFieldID(jRidePointClass,"hr","D");
        point.setValue(RideFile::hr,env->GetDoubleField(jRidePoint,jhrFeild));
        //qDebug() << "jhrFeild done";

        jfieldID jkmFeild = env->GetFieldID(jRidePointClass,"km","D");
        point.setValue(RideFile::km,env->GetDoubleField(jRidePoint,jkmFeild));
        //qDebug() << "jkmFeild done";

        jfieldID jlonFeild = env->GetFieldID(jRidePointClass,"lon","D");
        point.setValue(RideFile::lon,env->GetDoubleField(jRidePoint,jlonFeild));
        //qDebug() << "jlonFeild done";

        jfieldID jlatFeild = env->GetFieldID(jRidePointClass,"lat","D");
        point.setValue(RideFile::lat,env->GetDoubleField(jRidePoint,jlatFeild));
        //qDebug() << "jlatFeild done";

        jfieldID jsecsFeild = env->GetFieldID(jRidePointClass,"secs","D");
        point.setValue(RideFile::secs,env->GetDoubleField(jRidePoint,jsecsFeild));
        //qDebug() << "jsecsFeild done";

        jfieldID jcadFeild = env->GetFieldID(jRidePointClass,"cad","D");
        point.setValue(RideFile::cad,env->GetDoubleField(jRidePoint,jcadFeild));
        //qDebug() << "jcadFeild done";

        jfieldID jrcadFeild = env->GetFieldID(jRidePointClass,"rcad","D");
        point.setValue(RideFile::rcad,env->GetDoubleField(jRidePoint,jrcadFeild));
        //qDebug() << "jrcadFeild done";

        jfieldID jkphFeild = env->GetFieldID(jRidePointClass,"kph","D");
        point.setValue(RideFile::kph,env->GetDoubleField(jRidePoint,jkphFeild));
        //qDebug() << "jkphFeild done";

        jfieldID jwattsFeild = env->GetFieldID(jRidePointClass,"watts","D");
        point.setValue(RideFile::watts,env->GetDoubleField(jRidePoint,jwattsFeild));
        //qDebug() << "jwattsFeild done";

        jfieldID jaltFeild = env->GetFieldID(jRidePointClass,"alt","D");
        point.setValue(RideFile::alt,env->GetDoubleField(jRidePoint,jaltFeild));
        //qDebug() << "jaltFeild done";

        jfieldID jtempFeild = env->GetFieldID(jRidePointClass,"temp","D");
        point.setValue(RideFile::temp,env->GetDoubleField(jRidePoint,jtempFeild));
        //qDebug() << "jtempFeild done";

        rideFile->appendPoint(point);
    }

    jmethodID getRecIntSecsMethodId = env->GetMethodID(genrator, "getRecIntSecs", "()D");
    if( getRecIntSecsMethodId != NULL){
        qDebug() << "Got getRecIntSecs Method";
    }else {
        qDebug() << "Not able to get getRecIntSecs method";
    }
    jdouble recIntSecs = env->CallDoubleMethod(gen, getRecIntSecsMethodId);
    rideFile->setRecIntSecs(recIntSecs);

    jmethodID getSportMethodId = env->GetMethodID(genrator, "getSport", "()Ljava/lang/String;");
    if( getSportMethodId != NULL){
        qDebug() << "Got getSport Method";
    }else {
        qDebug() << "Not able to get getSport method";
    }
    jstring sport = (jstring) env->CallObjectMethod(gen, getSportMethodId);
    QString sportString = env->GetStringUTFChars(sport, 0);
    rideFile->setTag("Sport", sportString);

    qDebug() << "matricsinterface.h : jniConvertRidePoints : Returning from the method";
}
void jniGetMatricsToCalculate(JNIEnv *env,jobject gen){
    jclass genrator=env->GetObjectClass(gen);
    if( genrator != NULL){
        qDebug() << "Got com.svlabs.svt.matrics.SaarMatricsGenerator Object";
    }else{
         qDebug() << "Problem while gettig com.svlabs.svt.matrics.SaarMatricsGenerator Object";
    }
    jmethodID methodId=env->GetMethodID(genrator, "getAllMatrics", "()[Ljava/lang/String;");
    if( methodId != NULL){
        qDebug() << "Got getAllMatrics Method";
    }else {
         qDebug() << "Not able to get getAllMatrics method";
    }
    jobjectArray matricsToCal = (jobjectArray)env->CallObjectMethod(gen, methodId);
    if(matricsToCal != NULL){
        qDebug() << "Not able call the method";
    }else{
        qDebug() << "call to getAllMatrics done";
    }
    jsize length = env->GetArrayLength(matricsToCal);
    qDebug() << "Matrics to analysis : " << length;
    for (int i=0;i<length;i++) {
        jstring string = (jstring) (env->GetObjectArrayElement(matricsToCal, i));
        const char* eachMatrics = env->GetStringUTFChars(string,0);
        qDebug() << "Matrics got" << eachMatrics;
        testMatrics.append(eachMatrics);
        env->ReleaseStringUTFChars(string,eachMatrics);
        qDebug() << "After Release";
    }
}
