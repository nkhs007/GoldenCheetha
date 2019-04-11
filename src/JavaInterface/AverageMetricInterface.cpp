#include "AverageMetricInterface.h"
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
#include "Specification.h"

void
AverageMetrice::_intializeMatricValueStructure(JNIEnv *env){
    jniMatricValueStructure = new _MatricValue;
    jniMatricValueStructure->cls = env->FindClass("com/svlabs/svt/ride/MatricValue");
    if(jniMatricValueStructure->cls != NULL)
        qDebug() <<"sucessfully created com/svlabs/svt/ride/MatricValue";
    jniMatricValueStructure->constructortorID = env->GetMethodID(jniMatricValueStructure->cls, "<init>", "()V");
    if(jniMatricValueStructure->constructortorID != NULL){
        qDebug() << "sucessfully created MatricValue constructor   ";
    }
    jniMatricValueStructure->setKey = env->GetMethodID(jniMatricValueStructure->cls,"setKey","(Ljava/lang/String;)V");
    jniMatricValueStructure->setValue = env->GetMethodID(jniMatricValueStructure->cls,"setValue","(Ljava/lang/String;)V");
}

void
AverageMetrice::jniAquireRidePoints(JNIEnv *env,jobject gen,RideFile *rideFile){

    qDebug() << "averagematricsinterface.h : jniConvertRidePoints : Start converting";
    //com.svlabs.svt.matrics.SaarMatricsAverageGenerator
    jclass genrator=env->GetObjectClass(gen);
    if( genrator != NULL){
        qDebug() << "Got com.svlabs.svt.ride.RideItemData Object";
    }else{
         qDebug() << "Problem while gettig com.svlabs.svt.ride.RideItemData Object";
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

void
AverageMetrice::jniAquireAthleteData(JNIEnv *env,jobject gen){

    jclass genrator=env->GetObjectClass(gen);

    jmethodID getWeightMethodId = env->GetMethodID(genrator, "getWeight", "()D");
    if( getWeightMethodId != NULL){
        qDebug() << "Got getWeight Method";
    }else {
        qDebug() << "Not able to get getWeight method";
    }
    athleteWeight = env->CallDoubleMethod(gen, getWeightMethodId);

    jmethodID getHeightMethodId = env->GetMethodID(genrator, "getHeight", "()D");
    if( getHeightMethodId != nullptr){
        qDebug() << "Got getHeight Method";
    }else {
        qDebug() << "Not able to get getHeight method";
    }
    athleteHeight = env->CallDoubleMethod(gen, getHeightMethodId);

    jmethodID getMetricMethodId = env->GetMethodID(genrator, "isMetric", "()Z");
    if( getMetricMethodId != nullptr){
        qDebug() << "Got isMetric Method";
    }else {
        qDebug() << "Not able to get isMetric method";
    }
    isMetric = env->CallBooleanMethod(gen, getMetricMethodId);

}


void
AverageMetrice::jniAquireMatricsToCalculate(JNIEnv *env,jobject gen){
    //First clear the list of matrics to calculate

//    for (int i=0;i<requiredMatrics.size();i++) {
//        QString metric = requiredMatrics[i];
//        freelocale(metric)
//    }

    requiredMatrics.clear();

    qDebug() << "averagematricsinterface.h : jniGetMatricsToCalculate : Begin of the method";
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
        requiredMatrics.append(eachMatrics);
        env->ReleaseStringUTFChars(string,eachMatrics);
        //qDebug() << "After Release";
    }
    qDebug() << "matricsinterface.h : jniGetMatricsToCalculate : End of the method : testMatrics.size="<<requiredMatrics.size();
}


void
AverageMetrice::jniAquirePowerZone(JNIEnv *env,jobject gen, Zones *zones[2]){
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
void
AverageMetrice::jniAquireHeartZone(JNIEnv *env, jobject gen, HrZones *hrzones[2]){
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

void
AverageMetrice::jniAquirePaceZone(JNIEnv *env,jobject gen, PaceZones *pacezones[2]){

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

void
AverageMetrice::calculateOtherMetrics(JNIEnv *env,Athlete *athlete, jobject gen){
//    qDebug() << "*Creating PMC object*";

    jmethodID getAverageMethodId = env->GetMethodID(env->GetObjectClass(gen), "isAverage", "()Z");
    if( getAverageMethodId != nullptr){
        qDebug() << "Got isAverage Method";
    }else {
        qDebug() << "Not able to get isAverage method";
    }
    isAverage = env->CallBooleanMethod(gen, getAverageMethodId);

    if (isAverage == true) {
        AverageMetrice::calculateOtherMetricAverage(env,gen,athlete);
        return;
    }

    PMCData *pmc = athlete->getPMCFor("triscore");

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
            jobject jniLTSValue = env->NewObject(jniMatricValueStructure->cls, jniMatricValueStructure->constructortorID);
            env->CallObjectMethod(jniLTSValue,jniMatricValueStructure->setKey, env->NewStringUTF("CTL"));
            env->CallObjectMethod(jniLTSValue,jniMatricValueStructure->setValue,  env->NewStringUTF(std::to_string(lts).data()));
            env->CallObjectMethod(gen, jniAddRidePointMethod, jniLTSValue);
        }
        if (strncmp(nativeString,"ATL",3) == 0) {
            jobject jniSTSValue = env->NewObject(jniMatricValueStructure->cls, jniMatricValueStructure->constructortorID);
            env->CallObjectMethod(jniSTSValue,jniMatricValueStructure->setKey, env->NewStringUTF("ATL"));
            env->CallObjectMethod(jniSTSValue,jniMatricValueStructure->setValue, env->NewStringUTF(std::to_string(sts).data()));
            env->CallObjectMethod(gen, jniAddRidePointMethod, jniSTSValue);

        }
        if (strncmp(nativeString,"TSB",3) == 0) {
            jobject jniSBValue = env->NewObject(jniMatricValueStructure->cls, jniMatricValueStructure->constructortorID);
            env->CallObjectMethod(jniSBValue,jniMatricValueStructure->setKey, env->NewStringUTF("TSB"));
            env->CallObjectMethod(jniSBValue,jniMatricValueStructure->setValue, env->NewStringUTF(std::to_string(sb).data()));
            env->CallObjectMethod(gen, jniAddRidePointMethod, jniSBValue);

        }
        if (strncmp(nativeString,"RR",3) == 0) {
            jobject jniRRValue = env->NewObject(jniMatricValueStructure->cls, jniMatricValueStructure->constructortorID);
            env->CallObjectMethod(jniRRValue,jniMatricValueStructure->setKey, env->NewStringUTF("RR"));
            env->CallObjectMethod(jniRRValue,jniMatricValueStructure->setValue, env->NewStringUTF(std::to_string(rr).data()));
            env->CallObjectMethod(gen, jniAddRidePointMethod, jniRRValue);
        }
    }

}



void
AverageMetrice::setCalculatedValuesToRideItem(RideItem *rideItem, QHash<QString, RideMetricPtr> notmetrics){

    RideMetricFactory &factory = RideMetricFactory::instance();
    QHashIterator<QString, RideMetricPtr> i(notmetrics);
    QVector<double> metrics;
    metrics.resize(factory.allMetrics().size());
    factory.initialize();
    while (i.hasNext()) {
        i.next();
        RideMetricPtr rideMatPtr = i.value();
        QString key = i.key();
        const RideMetric *m = factory.rideMetric(key);
        metrics[m->index()] = rideMatPtr->value();
    }
    rideItem->setMmetrics(metrics);

}

void
AverageMetrice::calculateOtherMetricAverage(JNIEnv *env, jobject gen, Athlete *athlete){
    qDebug() << "calculating other metrics";
    PMCData *pmc = athlete->getPMCFor("triscore");

    int lowCTL=0, highCTL=0, lowATL=0, highATL=0, lowTSB=0, highTSB=0, lowRR=0,highRR=0;
    double avgCTL=0, avgATL=0, avgTSB=0, avgRR=0;
    int count=0;
    std::string ctl,atl,tsb,rr;
    std::string space = "";

//    int start=pmc->indexOf(athlete->rideCache->rides()[0]->dateTime.date());
//    int end=pmc->indexOf(athlete->rideCache->rides()[athlete->rideCache->rides().count() - 1]->dateTime.date());
    int start=pmc->indexOf(pmc->start());
    int end=pmc->indexOf(athlete->rideCache->rides()[athlete->rideCache->rides().count() - 1]->dateTime.date());
    if (start == -1) start = 0;
//    if (end==-1)
        end = pmc->lts().count()-1;

    bool first=true;
    int length = athlete->rideCache->rides().size();
     qDebug() << "start date is " << pmc->start();
     qDebug() << "ebd date is " << pmc->end();

     for (int i=start; i<end; i++) {

         double ctl = pmc->lts()[i];
         double atl = pmc->sts()[i];
         double tsb = pmc->sb()[i];
         double rr = pmc->rr()[i];

        qDebug() << "ctl for " << i << "is " << ctl;
        qDebug() << "atl for " << i<< "is " << atl;
        qDebug() << "tsb for " << i << "is " << tsb;
        qDebug() << "rr for " << i << "is " << rr;

        count++;
        avgCTL += ctl;
        avgATL += atl;
        avgTSB += tsb;
        avgRR += rr;

        if (first) {

            // initialise
            lowCTL = highCTL = ctl;
            lowATL = highATL = atl;
            lowTSB = highTSB = tsb;
            lowRR = highTSB = rr;
            first = false;

        } else {

            // lower/higher than we already got ?
            if (ctl < lowCTL) lowCTL=ctl;
            if (ctl > highCTL) highCTL=ctl;
            if (atl < lowATL) lowATL=atl;
            if (atl > highATL) highATL=atl;
            if (tsb < lowTSB) lowTSB=tsb;
            if (tsb > highTSB) highTSB=tsb;
            if (rr < lowRR) lowRR=rr;
            if (rr > highRR) highRR=rr;
        }
    }

    if (count) {
        avgCTL /= double(count);
        avgATL /= double(count);
        avgTSB /= double(count);
        avgRR /= double(count);
    }

    qDebug() << "ctl is " << avgCTL << "(" << lowCTL << " - " << highCTL << ")";
    qDebug() << "atl is " << avgATL << "(" << lowATL << " - " << highATL <<")";
    qDebug() << "tsb is " << avgTSB << "(" <<lowTSB << " - " << highTSB<<")";
    qDebug() << "rr is " << avgRR << "(" << lowRR << " - " << highRR << ")";

//ctl = space + avgCTL;


    ctl = space + std::to_string((int)avgCTL) + " ( " + std::to_string(lowCTL) + " - " + std::to_string(highCTL) + " )";
    atl = space + std::to_string((int)avgATL) + " ( " + std::to_string(lowATL) + " - " + std::to_string(highATL) + " )";
    tsb = space + std::to_string((int)avgTSB) + " ( " + std::to_string(lowTSB) + " - " + std::to_string(highTSB) + " )";
    rr = space + std::to_string((int)avgRR) + " ( " + std::to_string(lowRR) + " - " + std::to_string(highRR) + " )";

    jmethodID jniAddRidePointMethod = env->GetMethodID(env->GetObjectClass(gen),"addMatricValue","(Lcom/svlabs/svt/ride/MatricValue;)V");
    jsize metricLength = requiredMatrics.size();
    for (int i=0;i<metricLength;i++) {
        QByteArray metricName = requiredMatrics[i].toLocal8Bit();
          const char *metrichNameChar = metricName.data();
        if (strncmp(metrichNameChar,"CTL",3) == 0) {
            jobject jniLTSValue = env->NewObject(jniMatricValueStructure->cls, jniMatricValueStructure->constructortorID);
            env->CallObjectMethod(jniLTSValue,jniMatricValueStructure->setKey, env->NewStringUTF("CTL"));
            env->CallObjectMethod(jniLTSValue,jniMatricValueStructure->setValue,  env->NewStringUTF(ctl.data()));
            env->CallObjectMethod(gen, jniAddRidePointMethod, jniLTSValue);
        }
        if (strncmp(metrichNameChar,"ATL",3) == 0) {
            jobject jniSTSValue = env->NewObject(jniMatricValueStructure->cls, jniMatricValueStructure->constructortorID);
            env->CallObjectMethod(jniSTSValue,jniMatricValueStructure->setKey, env->NewStringUTF("ATL"));
            env->CallObjectMethod(jniSTSValue,jniMatricValueStructure->setValue,  env->NewStringUTF(atl.data()));
            env->CallObjectMethod(gen, jniAddRidePointMethod, jniSTSValue);

        }
        if (strncmp(metrichNameChar,"TSB",3) == 0) {
            jobject jniSBValue = env->NewObject(jniMatricValueStructure->cls, jniMatricValueStructure->constructortorID);
            env->CallObjectMethod(jniSBValue,jniMatricValueStructure->setKey, env->NewStringUTF("TSB"));
            env->CallObjectMethod(jniSBValue,jniMatricValueStructure->setValue,   env->NewStringUTF(tsb.data()));
            env->CallObjectMethod(gen, jniAddRidePointMethod, jniSBValue);

        }
        if (strncmp(metrichNameChar,"RR",3) == 0) {
            jobject jniRRValue = env->NewObject(jniMatricValueStructure->cls, jniMatricValueStructure->constructortorID);
            env->CallObjectMethod(jniRRValue,jniMatricValueStructure->setKey, env->NewStringUTF("RR"));
            env->CallObjectMethod(jniRRValue,jniMatricValueStructure->setValue,  env->NewStringUTF(rr.data()));
            env->CallObjectMethod(gen, jniAddRidePointMethod, jniRRValue);
        }
    }

}

void
AverageMetrice::jniAquireRideFileData(JNIEnv *env, jobject gen, Athlete *athlete) {

    jclass genrator=env->GetObjectClass(gen);
    if( genrator != NULL){
        qDebug() << "Got com.svlabs.svt.matrics.SaarMatricsGenerator Object";
    }else{
         qDebug() << "Problem while gettig com.svlabs.svt.matrics.SaarMatricsGenerator Object";
    }
    jmethodID methodId=env->GetMethodID(genrator, "getRideItemDatas", "()[Lcom/svlabs/svt/ride/RideItemData;");
    if( methodId != NULL){
        qDebug() << "Got getRidePoints Method";
    }else {
         qDebug() << "Not able to get getRidePoints method";
    }
    jobjectArray jRideItemData = (jobjectArray)env->CallObjectMethod(gen, methodId);
//

    jsize length = env->GetArrayLength(jRideItemData);
    qDebug() << "Points to analysis : " << length;
    for (int i=0;i<length;i++) {
        jobject jRideItem = env->GetObjectArrayElement(jRideItemData, i);
        RideFile *file = new RideFile();
        jniAquireRidePoints(env,jRideItem,file);
        RideItem *rideItem = new RideItem(file, athlete->context);
        rideItem->isRun = file->isRun();
        rideItem->isSwim = file->isSwim();
        rideItem->setStartTime(file->startTime());
        rideItem->planned = false;

        rideItem->zoneRange = athlete->zones(false)->whichRange(file->startTime().date());
        rideItem->hrZoneRange = athlete->hrZones(false)->whichRange(file->startTime().date());
        rideItem->paceZoneRange = athlete->paceZones(false)->whichRange(file->startTime().date());

        rideItem->ride()->context = athlete->context;
        rideItem->weight = athleteWeight;


         QHash<QString, RideMetricPtr> notmetrics = RideMetric::computeMetrics(rideItem, Specification(),requiredMatrics);
         setCalculatedValuesToRideItem(rideItem, notmetrics);

         athlete->rideCache->rides().append(rideItem);
    }
}

void
AverageMetrice::calculateAverageValuesOfAllMetrices(JNIEnv *env, jobject gen, Athlete *athlete){
    int noOfMetrics = requiredMatrics.size();
    qDebug() << "number of metric to calculate " << noOfMetrics;
    jmethodID jniAddRidePointMethod = env->GetMethodID(env->GetObjectClass(gen),"addMatricValue","(Lcom/svlabs/svt/ride/MatricValue;)V");

    for (int i=0;i<noOfMetrics;i++) {
        QString aggregate =  athlete->rideCache->getAggregate(requiredMatrics[i],*new Specification(),isMetric);
        if (aggregate == requiredMatrics[i] + " unknown") {
            continue;
        }
        jobject jniLTSValue = env->NewObject(jniMatricValueStructure->cls, jniMatricValueStructure->constructortorID);
        QByteArray metricName = requiredMatrics[i].toLocal8Bit();
        const char *metrichNameChar = metricName.data();
        QByteArray value = aggregate.toLocal8Bit();
        const char *valueChar = value.data();
        env->CallObjectMethod(jniLTSValue,jniMatricValueStructure->setKey,  env->NewStringUTF(metrichNameChar));
        env->CallObjectMethod(jniLTSValue,jniMatricValueStructure->setValue,  env->NewStringUTF(valueChar));
        env->CallObjectMethod(gen, jniAddRidePointMethod, jniLTSValue);

        qDebug() << requiredMatrics[i] << ":" << valueChar;
    }
    calculateOtherMetrics(env, athlete, gen);
}

void
AverageMetrice::createPmcGraphData(JNIEnv *env, jobject gen, Athlete *athlete){

     QString scoreType = "triscore";

     PMCData *pmcData = athlete->context->athlete->getPMCFor(scoreType);

     jclass pmcValueClass =  env->FindClass("com/svlabs/svt/ride/PmcGraphValue");
     if (NULL == pmcValueClass) {
         qDebug() << "pmcValueClass not available";
     }
     jmethodID pmcValueClassConstructor = env->GetMethodID(pmcValueClass, "<init>", "()V");
     if (NULL == pmcValueClassConstructor) {
         qDebug() << "pmcValueClassConstructor not available";
     }
     jmethodID setDateMethodId = env->GetMethodID(pmcValueClass,"setDate","(J)V");
     if (NULL == setDateMethodId) {
         qDebug() << "setDateMethodId not available";
     }
     jmethodID setCtlMethodId = env->GetMethodID(pmcValueClass,"setCtl","(D)V");
     if (NULL == setCtlMethodId) {
         qDebug() << "setCtlMethodId not available";
     }
     jmethodID setAtlMethodId = env->GetMethodID(pmcValueClass,"setAtl","(D)V");
     if (NULL == setAtlMethodId) {
         qDebug() << "setAtlMethodId not available";
     }
     jmethodID setTsbMethodId = env->GetMethodID(pmcValueClass,"setTsb","(D)V");
     if (NULL == setTsbMethodId) {
         qDebug() << "setTsbMethodId not available";
     }
     jmethodID setRrMethodId = env->GetMethodID(pmcValueClass,"setRr","(D)V");
     if (NULL == setRrMethodId) {
         qDebug() << "setRrMethodId not available";
     }
     jmethodID addPmcGraphValuesMethodId = env->GetMethodID(env->GetObjectClass(gen), "addPmcGraphValues", "(Lcom/svlabs/svt/ride/PmcGraphValue;)V");
     if (NULL == addPmcGraphValuesMethodId) {
         qDebug() << "addPmcGraphValuesMethodId not available";
     }
     for (QDate date=pmcData->start(); date <= QDate::currentDate(); date = date.addDays(1)) {
         jobject jniPmcValueObject = env->NewObject(pmcValueClass, pmcValueClassConstructor);

         env->CallLongMethod(jniPmcValueObject, setDateMethodId, QDateTime(date).toMSecsSinceEpoch());
         env->CallDoubleMethod(jniPmcValueObject, setCtlMethodId, pmcData->lts(date));
         env->CallDoubleMethod(jniPmcValueObject, setAtlMethodId, pmcData->sts(date));
         env->CallDoubleMethod(jniPmcValueObject, setTsbMethodId, pmcData->sb(date));
         env->CallDoubleMethod(jniPmcValueObject, setRrMethodId, pmcData->rr(date));

         env->CallObjectMethod(gen, addPmcGraphValuesMethodId, jniPmcValueObject);

//        qDebug() << "Date is " << date;
//        double ctl = pmcData->lts(date);
//        double atl = pmcData->sts(date);
//        double tsb = pmcData->sb(date);
//        double rr = pmcData->rr(date);

//        qDebug() << "CTL :" << ctl;
//        qDebug() << "ATL :" << atl;
//        qDebug() << "TSB :" << tsb;
//        qDebug() << "RR :" << rr;


     }

}





