#include "com_svlabs_svt_matrics_SaarPmcGraph.h"
#include "QDebug"
#include "AverageMetricInterface.h"

void Java_com_svlabs_svt_matrics_SaarPmcGraph_analysePmcGraphData(JNIEnv *env, jobject pmcGen){

    qDebug() << "Inside PMC data preperation for PMC graph"<< env << pmcGen;

    AverageMetrice *avgMetric = new AverageMetrice();

    Zones *zones[2];
    HrZones *hrzones[2];
    PaceZones *pacezones[2];

     avgMetric->jniAquirePowerZone(env, pmcGen, zones);
     avgMetric->jniAquireHeartZone(env, pmcGen, hrzones);
     avgMetric->jniAquirePaceZone(env, pmcGen, pacezones);

//     avgMetric.jniAquireMatricsToCalculate(env,gen);
     avgMetric->jniAquireAthleteData(env,pmcGen);

     int* firstarg = new int(0);
     QApplication *qApplication = new QApplication(*firstarg, NULL);

     Context *context = new Context();

     Athlete *athlete = new Athlete(context, zones, hrzones, pacezones);
     athlete->setAtheletWeight(avgMetric->athleteWeight);
     athlete->setAthleteHeight(avgMetric->athleteHeight);

     avgMetric->requiredMatrics.append("skiba_bike_score");
     avgMetric->requiredMatrics.append("triscore");
     avgMetric->requiredMatrics.append("swimscore");
     avgMetric->requiredMatrics.append("govss");

     avgMetric->jniAquireRideFileData(env,pmcGen,athlete);

     avgMetric->createPmcGraphData(env,pmcGen,athlete);

     qApplication->exit(0);
     qApplication->quit();
     delete context;
     delete firstarg;
     delete avgMetric->jniMatricValueStructure;
     delete avgMetric;
     free(athlete);

     qDebug() << "Exiting PMC data preperation for PMC graph";
}

