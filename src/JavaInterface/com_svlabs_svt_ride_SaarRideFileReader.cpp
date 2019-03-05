#include "com_svlabs_svt_ride_SaarRideFileReader.h"
#include <QDebug>
#include <QString>
#include "RideFile.h"
#include "FitRideFile.h"
#include <QFile>
#include <QList>
#include "Settings.h"
#include <vector>
#include "ridesinterface.h"



jobject Java_com_svlabs_svt_ride_SaarRideFileReader_readFile(JNIEnv * env, jobject, jstring fileName)
{
    qDebug()<<"Call in RideFile Read";
    QList<RideFile*> rides;
    QStringList errors;
    const char * inString = env->GetStringUTFChars(fileName, NULL);
    QFile fitFile(inString);
    env->ReleaseStringUTFChars(fileName,inString);

    RideFile *ride = RideFileFactory::instance().openRideFile(NULL, fitFile, errors, &rides);
    if(ride){
        qDebug()<<"Has Ride";
        createSVTRideFileJavaObject(env);
        createRideFilePointJavaObject(env);

        qDebug() << "Trying to create jniRideFile object ";
        jobject jniRideFile = env->NewObject(jniRideFileStruct->cls, jniRideFileStruct->constructorId);
        qDebug() << "sucessfully created jniRideFile object   ";

        QDateTime starttime = ride->startTime();
        jlong starttimeint = starttime.toMSecsSinceEpoch();
        qDebug() << "Starttime--- " << starttime << " starttimeint --- "<< starttimeint;
        env->SetLongField(jniRideFile, jniRideFileStruct->starttime, starttimeint);

        double recintsecs = ride->recIntSecs();
        qDebug() << "recintsecs--- " << recintsecs;
        env->SetDoubleField(jniRideFile, jniRideFileStruct->recintsecs, recintsecs);

        QMap<QString,QString> tags = ride->tags();
        qDebug() << "tags--- " << tags;

        jclass mapClass = env->FindClass("java/util/HashMap");
        qDebug() << "mapClass created";
        jsize map_len = tags.size();
        qDebug() << "tags size  " << map_len;
        jmethodID init = env->GetMethodID(mapClass, "<init>", "(I)V");
        if( init != NULL ) {
             qDebug() << "mapClass initiatilzed  ";
        }
        jobject hashMap = env->NewObject(mapClass, init, map_len);
        if( hashMap != NULL ) {
             qDebug() << "hashMap object created";
        }
        jmethodID put = env->GetMethodID(mapClass, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
        if( put != NULL ) {
             qDebug() << "got hashMap object put method";
        }
        qDebug() << "Before for loop";
        for (auto eachkey: tags.keys()) {
            //if( eachkey == "Sport") {
                qDebug() << "Inside for looop " << eachkey;
                const char * key = eachkey.toStdString().c_str();
                jstring keyString = env->NewStringUTF(key);
                qDebug() << "key got converted to char array " << key;
                qDebug() << "key got converted to jstring " << keyString;
                const char * value = tags.value(eachkey).toStdString().c_str();
                jstring valueString = env->NewStringUTF(value);
                qDebug() << "value got converted to char array" << value;
                qDebug() << "value got converted to jstring" << valueString;
                env->CallObjectMethod(hashMap, put, keyString, valueString);
                qDebug() << "Data added to the map";
            //}
        }
        qDebug() << "Trying to set the tags";
        env->SetObjectField(jniRideFile, jniRideFileStruct->tags, hashMap);

        QVector<RideFilePoint*> allPoints = ride->dataPoints();
        jobjectArray jiniRidePointArray = env->NewObjectArray(allPoints.size(), jniRidePointStruct->cls, NULL);
        qDebug()<<"Created  jiniRidePointArray";
        for (int i=0;i<allPoints.size();i++) {
             RideFilePoint *eachPoint = allPoints.at(i);
             jobject jniRidePoint = env->NewObject(jniRidePointStruct->cls, jniRidePointStruct->constructortorID);
             env->SetDoubleField(jniRidePoint,jniRidePointStruct->hr,eachPoint->hr);
             env->SetDoubleField(jniRidePoint,jniRidePointStruct->secs,eachPoint->secs);
             env->SetDoubleField(jniRidePoint,jniRidePointStruct->km,eachPoint->km);
             env->SetDoubleField(jniRidePoint,jniRidePointStruct->lon,eachPoint->lon);
             env->SetDoubleField(jniRidePoint,jniRidePointStruct->lat,eachPoint->lat);
             env->SetDoubleField(jniRidePoint,jniRidePointStruct->cad,eachPoint->cad);
             env->SetDoubleField(jniRidePoint,jniRidePointStruct->rcad,eachPoint->rcad);
             env->SetDoubleField(jniRidePoint,jniRidePointStruct->kph,eachPoint->kph);
             env->SetDoubleField(jniRidePoint,jniRidePointStruct->watts,eachPoint->watts);
             env->SetDoubleField(jniRidePoint,jniRidePointStruct->alt,eachPoint->alt);
             env->SetDoubleField(jniRidePoint,jniRidePointStruct->temp,eachPoint->temp);
             env->SetObjectArrayElement(jiniRidePointArray, i, jniRidePoint);
             //qDebug()<<"Adding :"<<i;
        }

        env->SetObjectField(jniRideFile, jniRideFileStruct->samples, jiniRidePointArray);
        qDebug() << "sucessfully added jniRideFile samples   ";
        qDebug()<<"Returning :";
        return jniRideFile;
    }
    qDebug()<<"No Ride";
}
