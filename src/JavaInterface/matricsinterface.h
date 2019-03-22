#ifndef MATRICSINTERFACE_H
#define MATRICSINTERFACE_H

#endif // MATRICSINTERFACE_H
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

QStringList testMatrics;
typedef struct _MatricValue {
    jclass cls;
    jmethodID constructortorID;
    jmethodID setKey;
    jmethodID setValue;
}_MatricValue;

_MatricValue * jniMatricValueStruct = NULL;

double weight;
double height;

void _initMatricValueStructure(JNIEnv *env){
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

        jfieldID jslopeFeild = env->GetFieldID(jRidePointClass,"slope","D");
        point.setValue(RideFile::slope,env->GetDoubleField(jRidePoint,jslopeFeild));
        //qDebug() << "jslopeFeild done";

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


    jmethodID getWeightMethodId = env->GetMethodID(genrator, "getWeight", "()D");
    if( getWeightMethodId != NULL){
        qDebug() << "Got getWeight Method";
    }else {
        qDebug() << "Not able to get getWeight method";
    }
    weight = env->CallDoubleMethod(gen, getWeightMethodId);

    jmethodID getHeightMethodId = env->GetMethodID(genrator, "getHeight", "()D");
    if( getHeightMethodId != nullptr){
        qDebug() << "Got getHeight Method";
    }else {
        qDebug() << "Not able to get getHeight method";
    }
    height = env->CallDoubleMethod(gen, getHeightMethodId);

     jmethodID getDateMethodId = env->GetMethodID(genrator, "getDate", "()J");
     if (!getDateMethodId) {
         qDebug() << "date not available";
     }
     long dateTime = env->CallLongMethod(gen, getDateMethodId);
     QDateTime date = QDateTime();
     date.setMSecsSinceEpoch(dateTime);
     rideFile->setStartTime(date);

     qDebug() << "date is " << date.toString();

    qDebug() << "matricsinterface.h : jniConvertRidePoints : Returning from the method";
}

void jniGetAthleteData(JNIEnv *env,jobject gen){

    jclass genrator=env->GetObjectClass(gen);

    jmethodID getWeightMethodId = env->GetMethodID(genrator, "getWeight", "()D");
    if( getWeightMethodId != NULL){
        qDebug() << "Got getWeight Method";
    }else {
        qDebug() << "Not able to get getWeight method";
    }
    weight = env->CallDoubleMethod(gen, getWeightMethodId);

    jmethodID getHeightMethodId = env->GetMethodID(genrator, "getHeight", "()D");
    if( getHeightMethodId != nullptr){
        qDebug() << "Got getHeight Method";
    }else {
        qDebug() << "Not able to get getHeight method";
    }
    height = env->CallDoubleMethod(gen, getHeightMethodId);
}


void jniGetMatricsToCalculate(JNIEnv *env,jobject gen){
    //First clear the list of matrics to calculate
    testMatrics.clear();
    qDebug() << "matricsinterface.h : jniGetMatricsToCalculate : Begin of the method";
    jclass genrator=env->GetObjectClass(gen);
    if( genrator != NULL){
        //qDebug() << "Got com.svlabs.svt.matrics.SaarMatricsGenerator Object";
    }else{
        qDebug() << "Problem while gettig com.svlabs.svt.matrics.SaarMatricsGenerator Object";
    }
    jmethodID methodId=env->GetMethodID(genrator, "getAllMatrics", "()[Ljava/lang/String;");
    if( methodId != NULL){
         //qDebug() << "Got getAllMatrics Method";
    }else {
         qDebug() << "Not able to get getAllMatrics method";
    }
    jobjectArray matricsToCal = (jobjectArray)env->CallObjectMethod(gen, methodId);
    if(matricsToCal != NULL){
        //qDebug() << "call to getAllMatrics done";
    }else{
        qDebug() << "Not able call the method";
    }
    jsize length = env->GetArrayLength(matricsToCal);
    //qDebug() << "Matrics to analysis : " << length;
    for (int i=0;i<length;i++) {
        jstring string = (jstring) (env->GetObjectArrayElement(matricsToCal, i));
        const char* eachMatrics = env->GetStringUTFChars(string,0);
        //qDebug() << "Matrics got" << eachMatrics;
        testMatrics.append(eachMatrics);
        env->ReleaseStringUTFChars(string,eachMatrics);
        //qDebug() << "After Release";
    }
    qDebug() << "matricsinterface.h : jniGetMatricsToCalculate : End of the method : testMatrics.size="<<testMatrics.size();
}


void jniConvertPowerZone(JNIEnv *env,jobject gen, Zones *zones[2]){
    jclass genrator = env->GetObjectClass(gen);
    jmethodID getPowerZoneMethodId = env->GetMethodID(genrator, "getPowerZone", "()Lcom/svlabs/svt/ride/UserThreshold;");
    if (NULL == getPowerZoneMethodId) {
        qDebug() << "getPowerZoneMethodId is NULL";
    }
    jobject powerZoneObject = (jobject)env->CallObjectMethod(gen, getPowerZoneMethodId);
    if (NULL == powerZoneObject) {
        qDebug() << "******************No Power Zone Data available******************";
        return;
    }
    jclass powerZoneClass = env->GetObjectClass(powerZoneObject);
    jmethodID getThresholdMethodId=env->GetMethodID(powerZoneClass, "getThresholdValue", "()D");
    jmethodID getZonesMethodId=env->GetMethodID(powerZoneClass, "getZones", "()[Lcom/svlabs/svt/ride/UserThresholdZone;");
    if (NULL == getZonesMethodId) {
        qDebug() << "getZonesMethodId is NULL";
    }
    jobjectArray jZones = (jobjectArray)env->CallObjectMethod(powerZoneObject, getZonesMethodId);

    for (int i=0; i < 2; i++) {
       zones[i] = new Zones(i>0);
       int pNum = zones[i]->addZoneRange(*new QDate(1900,1,1),(int)env->CallDoubleMethod(powerZoneObject, getThresholdMethodId),(int)env->CallDoubleMethod(powerZoneObject, getThresholdMethodId),20000,1000);
       QList<ZoneInfo> zoneInfo = QList<ZoneInfo>();
       jsize length = env->GetArrayLength(jZones);
       qDebug() << "Power zone length : " << length;
       for (int i=0;i<length;i++) {

          jobject jZoneValue = env->GetObjectArrayElement(jZones, i);
          jclass jZoneValueClass = env->GetObjectClass(jZoneValue);
          jmethodID jNameMethodId = env->GetMethodID(jZoneValueClass,"getName","()Ljava/lang/String;");
          jmethodID jMinValueMethodId = env->GetMethodID(jZoneValueClass,"getMinValue","()D");
          jmethodID jMaxValueMethodId = env->GetMethodID(jZoneValueClass,"getMaxValue","()D");
          jstring jName = (jstring) env->CallObjectMethod(jZoneValue, jNameMethodId);
          QString name = env->GetStringUTFChars(jName, 0);

          zoneInfo.append(*new ZoneInfo(name, name, (int)env->CallDoubleMethod(jZoneValue, jMinValueMethodId), (int)env->CallDoubleMethod(jZoneValue, jMaxValueMethodId)));
       }
        zones[i]->getZoneRange(pNum).zones = zoneInfo;
    }
}


// Heartrate Zones for Bike & Run
void jniConvertHeartZone(JNIEnv *env, jobject gen, HrZones *hrzones[2]){
    jclass genrator = env->GetObjectClass(gen);
    jmethodID hrZonemethodId = env->GetMethodID(genrator, "getHeartZone", "()Lcom/svlabs/svt/ride/UserThreshold;");
    jobject hrZoneObject = env->CallObjectMethod(gen, hrZonemethodId);
    if (NULL == hrZoneObject) {
        qDebug() << "******************No Heart Zone Data available******************";
        return;
    }
    jclass hrZoneClass = env->GetObjectClass(hrZoneObject);
    jmethodID getThresholdMethodId = env->GetMethodID(hrZoneClass, "getThresholdValue", "()D");
    jmethodID getMinMethodId = env->GetMethodID(hrZoneClass, "getMinValue", "()D");
    jmethodID getMaxMethodId = env->GetMethodID(hrZoneClass, "getMaxValue", "()D");
    jmethodID getZonesMethodId = env->GetMethodID(hrZoneClass, "getZones", "()[Lcom/svlabs/svt/ride/UserThresholdZone;");
    jobjectArray jZones = (jobjectArray) env->CallObjectMethod(hrZoneObject, getZonesMethodId);
    for (int i=0; i < 2; i++) {
       hrzones[i] = new HrZones(i>0);
       QList<HrZoneInfo> hrZoneInfos= QList<HrZoneInfo>();
       int num = hrzones[i]->addHrZoneRange(*new QDate(1900,1,1), (int)env->CallDoubleMethod(hrZoneObject, getThresholdMethodId), (int)env->CallDoubleMethod(hrZoneObject, getMinMethodId),  (int)env->CallDoubleMethod(hrZoneObject, getMaxMethodId));
       QList<PaceZoneInfo> paceZoneInfo = QList<PaceZoneInfo>();
       jsize length = env->GetArrayLength(jZones);
       qDebug() << "heart zone length : " << length;
       for (int i=0;i<length;i++) {
          jobject jZoneValue =  env->GetObjectArrayElement(jZones, i);
          jclass jZoneValueClass =  env->GetObjectClass(jZoneValue);
          jmethodID jNameMethodId = env->GetMethodID(jZoneValueClass,"getName","()Ljava/lang/String;");
          jmethodID jMinValueMethodId = env->GetMethodID(jZoneValueClass, "getMinValue", "()D");
          jmethodID jMaxValueMethodId = env->GetMethodID(jZoneValueClass, "getMaxValue", "()D");
          jstring jName = (jstring) env->CallObjectMethod(jZoneValue, jNameMethodId);
          QString name = env->GetStringUTFChars(jName, 0);
          hrZoneInfos.append(*new HrZoneInfo(name, name, (int)env->CallDoubleMethod(jZoneValue, jMinValueMethodId),(int)env->CallDoubleMethod(jZoneValue, jMaxValueMethodId),0.0));
       }
       hrzones[i]->getHrZoneRange(num).zones = hrZoneInfos;
    }
}

void jniConvertPaceZone(JNIEnv *env,jobject gen, PaceZones *pacezones[2]){

    jclass genrator=env->GetObjectClass(gen);
    jmethodID methodId=env->GetMethodID(genrator, "getPaceZone", "()Lcom/svlabs/svt/ride/UserThreshold;");
    jobject jPaceZone = (jobject)env->CallObjectMethod(gen, methodId);
    if (NULL == jPaceZone) {
        qDebug() << "******************No Pace Zone Data available******************";
        return;
    }
    jclass jZoneClass = env->GetObjectClass(jPaceZone);
    jmethodID getThresholdMethodId=env->GetMethodID(jZoneClass, "getThresholdValue", "()D");
    jmethodID getZonesMethodId=env->GetMethodID(jZoneClass, "getZones", "()[Lcom/svlabs/svt/ride/UserThresholdZone;");
    jobjectArray jZones = (jobjectArray)env->CallObjectMethod(jPaceZone, getZonesMethodId);

    // Pace Zones for Run & Swim
    for (int i=0; i < 2; i++) {
        pacezones[i] = new PaceZones(i>0);
        int paceNum = pacezones[i]->addZoneRange(*new QDate(1900,1,1), (int)env->CallDoubleMethod(jPaceZone, getThresholdMethodId));
        QList<PaceZoneInfo> paceZoneInfo = QList<PaceZoneInfo>();
        jsize length = env->GetArrayLength(jZones);
        qDebug() << "Pace zone length : " << length;
        for (int i=0;i<length;i++) {

           jobject jZoneValue = env->GetObjectArrayElement(jZones, i);
           jclass jZoneValueClass = env->GetObjectClass(jZoneValue);
           jmethodID jNameMethodId = env->GetMethodID(jZoneValueClass,"getName","()Ljava/lang/String;");
           jmethodID jMinValueMethodId = env->GetMethodID(jZoneValueClass,"getMinValue","()D");
           jmethodID jMaxValueMethodId = env->GetMethodID(jZoneValueClass,"getMaxValue","()D");

           jstring jName = (jstring) env->CallObjectMethod(jZoneValue, jNameMethodId);
           QString name = env->GetStringUTFChars(jName, 0);
            paceZoneInfo.append(*new PaceZoneInfo(name, name, env->CallDoubleMethod(jZoneValue, jMinValueMethodId),env->CallDoubleMethod(jZoneValue, jMaxValueMethodId)));
        }


        pacezones[i]->getZoneRange(paceNum).zones = paceZoneInfo;
    }
}

void calculateMetrics(JNIEnv *env,Context *context, jobject gen,RideItem *rideItem, QHash<QString, RideMetricPtr> notmetrics){
//    qDebug() << "*Creating PMC object*";

    const RideMetricFactory &factory = RideMetricFactory::instance();
    QHashIterator<QString, RideMetricPtr> i(notmetrics);
    QVector<double> metrics;
    metrics.resize(factory.allMetrics().size());
    while (i.hasNext()) {
        i.next();
        RideMetricPtr rideMatPtr = i.value();
        QString key = i.key();
        const RideMetric *m = factory.rideMetric(key);
        metrics[m->index()] = rideMatPtr->value();
    }
    rideItem->setMmetrics(metrics);

    PMCData *pmc = context->athlete->getPMCFor(
                                rideItem->isSwim ? "swimscore" :
                                rideItem->isRun ? "govss" :
                                "coggan_tss");

    qDebug() << "start -" << pmc->start().toString();
    double lts = pmc->lts(pmc->start().addDays(1));
    double sts = pmc->sts(pmc->start().addDays(1));
    double rr = pmc->rr(pmc->start().addDays(1));
    double sb = pmc->sb(pmc->start().addDays(1));

    qDebug() << "*****LTS-" << lts << "*****";
    qDebug() << "*****STS-" << sts << "*****";
    qDebug() << "*****RR-" << rr << "*****";
    qDebug() << "*****SB-" << sb << "*****";

    jmethodID getAllMatricsMethodId=env->GetMethodID(env->GetObjectClass(gen), "getAllMatrics", "()[Ljava/lang/String;");

    jobjectArray matricsToCal = (jobjectArray)env->CallObjectMethod(gen, getAllMatricsMethodId);

    jmethodID jniAddRidePointMethod = env->GetMethodID(env->GetObjectClass(gen),"addMatricValue","(Lcom/svlabs/svt/ride/MatricValue;)V");
    jsize length = env->GetArrayLength(matricsToCal);
    for (int i=0;i<length;i++) {
        jstring string = (jstring) (env->GetObjectArrayElement(matricsToCal, i));
        const char *nativeString = env->GetStringUTFChars(string, 0);
        qDebug() << nativeString;
        if (strncmp(nativeString,"CTL",3) == 0) {
            jobject jniLTSValue = env->NewObject(jniMatricValueStruct->cls, jniMatricValueStruct->constructortorID);
            env->CallObjectMethod(jniLTSValue,jniMatricValueStruct->setKey, env->NewStringUTF("CTL"));
            env->CallObjectMethod(jniLTSValue,jniMatricValueStruct->setValue, lts);
            env->CallObjectMethod(gen, jniAddRidePointMethod, jniLTSValue);
        }
        if (strncmp(nativeString,"ATL",3) == 0) {
            jobject jniSTSValue = env->NewObject(jniMatricValueStruct->cls, jniMatricValueStruct->constructortorID);
            env->CallObjectMethod(jniSTSValue,jniMatricValueStruct->setKey, env->NewStringUTF("ATL"));
            env->CallObjectMethod(jniSTSValue,jniMatricValueStruct->setValue, sts);
            env->CallObjectMethod(gen, jniAddRidePointMethod, jniSTSValue);

        }
        if (strncmp(nativeString,"TSB",3) == 0) {
            jobject jniSBValue = env->NewObject(jniMatricValueStruct->cls, jniMatricValueStruct->constructortorID);
            env->CallObjectMethod(jniSBValue,jniMatricValueStruct->setKey, env->NewStringUTF("TSB"));
            env->CallObjectMethod(jniSBValue,jniMatricValueStruct->setValue, sb);
            env->CallObjectMethod(gen, jniAddRidePointMethod, jniSBValue);

        }
        if (strncmp(nativeString,"RR",3) == 0) {
            jobject jniRRValue = env->NewObject(jniMatricValueStruct->cls, jniMatricValueStruct->constructortorID);
            env->CallObjectMethod(jniRRValue,jniMatricValueStruct->setKey, env->NewStringUTF("RR"));
            env->CallObjectMethod(jniRRValue,jniMatricValueStruct->setValue, rr);
            env->CallObjectMethod(gen, jniAddRidePointMethod, jniRRValue);
        }
    }






}



void setCalculatedValuesInRideItem(RideItem *rideItem, QHash<QString, RideMetricPtr> notmetrics){

    const RideMetricFactory &factory = RideMetricFactory::instance();
    QHashIterator<QString, RideMetricPtr> i(notmetrics);
    QVector<double> metrics;
    metrics.resize(factory.allMetrics().size());
    while (i.hasNext()) {
        i.next();
        RideMetricPtr rideMatPtr = i.value();
        QString key = i.key();
        const RideMetric *m = factory.rideMetric(key);
        metrics[m->index()] = rideMatPtr->value();
    }
    rideItem->setMmetrics(metrics);
}

//void jniGetRideFileData(JNIEnv *env, jobject gen, Athlete *athlete) {

//    jclass genrator=env->GetObjectClass(gen);
//    if( genrator != NULL){
//        qDebug() << "Got com.svlabs.svt.matrics.SaarMatricsGenerator Object";
//    }else{
//         qDebug() << "Problem while gettig com.svlabs.svt.matrics.SaarMatricsGenerator Object";
//    }
//    jmethodID methodId=env->GetMethodID(genrator, "getRideItemDatas", "()[Lcom/svlabs/svt/ride/RideItemData;");
//    if( methodId != NULL){
//        qDebug() << "Got getRidePoints Method";
//    }else {
//         qDebug() << "Not able to get getRidePoints method";
//    }
//    jobjectArray jRideItemData = (jobjectArray)env->CallObjectMethod(gen, methodId);
////

//    jsize length = env->GetArrayLength(jRideItemData);
//    qDebug() << "Points to analysis : " << length;
//    for (int i=0;i<length;i++) {
//        jobject jRideItem = env->GetObjectArrayElement(jRideItemData, i);
//        jclass jRideItemClass = env->GetObjectClass(jRideItem);
//        RideFile *file;
//        MATRICSINTERFACE_H::jniConvertRidePoints(env,gen,file);
//        RideItem *rideItem = new RideItem(file, athlete->context);
//        rideItem->isRun = file->isRun();
//        rideItem->isSwim = file->isSwim();
//        rideItem->setStartTime(file->startTime());
//        rideItem->planned = false;

//        rideItem->zoneRange = athlete->zones(false)->whichRange(file->startTime().date());
//        rideItem->hrZoneRange = athlete->hrZones(false)->whichRange(file->startTime().date());
//        rideItem->paceZoneRange = athlete->paceZones(false)->whichRange(file->startTime().date());

//        rideItem->ride()->context = athlete->context;
//        rideItem->weight = weight;

//         QHash<QString, RideMetricPtr> notmetrics = RideMetric::computeMetrics(rideItem, Specification(),testMatrics);
//         MATRICSINTERFACE_H::setCalculatedValuesInRideItem(rideItem, notmetrics);

//         athlete->rideCache->rides().append(rideItem);
//    }

//}
