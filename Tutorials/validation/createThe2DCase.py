import numpy as np
import sys
import os
#########################################################################################################################
#Based on Huang 2007
print "Hi Dear research Team. This is automated tutorial creator. \nGood luck"

#Geometry inputs
airXLength=1.
airYLength=1.
airZLength=0.1*airXLength

pcmXLength=0.2
pcmYLength=airYLength
pcmZLength=airZLength

airNx=40
airNy=40
pcmNx=30
pcmNy=airNy

#!!!!!!!!!!!!!!! Properties
PREF=1e5
#!!!! pcm
fluxRightPcm=-1.

pcmLiqT=29.4+273
pcmSolT=28.6+273
pcmLiqK=0.15
pcmSolK=0.25
pcmLiqCp=1770.
pcmSolCp=1940.
pcmLiqRho=785.
pcmSolRho=940.
pcmLiqMu=3.74e-3
pcmLiqNu=pcmLiqMu/pcmLiqRho
pcmLatentHeat=202e3

airRho=1.204
airMolWeight=28.9
airCp=1007
airMu=1.825e-5
airNu=airMu/airRho
airPR=0.71

pcmLiqAlph=pcmLiqK/(pcmLiqRho*pcmLiqCp)
pcmSolAlph=pcmSolK/(pcmSolRho*pcmSolCp)

kPcm=2.
TGradRightPcm=-fluxRightPcm/kPcm

XMINaIR=-airXLength
XMAXaIR=XMINaIR+airXLength
YMINaIR=0.
YMAXaIR=YMINaIR+airYLength

XMAXpCM=XMAXaIR+pcmXLength
YMINpCM=YMINaIR
YMAXpCM=YMAXaIR
ZMIN=-airZLength*0.5
ZMIN=-airZLength*0.5
ZMAX=ZMIN+airZLength

#Time Settings
DELTAT=0.002
ENDTIME=120
WRITEINTERVAL=10

#Number of processors
NProcessor=10

  
#########################################################################################################################
case="."

print "Current directory is: "
os.system('pwd')
print "WARNING: every previous results in the current directory may be deleted, so answer carefully\n"
print "Do you want to restart the simulation? yes/no "
answer=input()

startFrom="firstTime"

if(answer == "no"):
    startFrom="latestTime"
elif(answer == "yes"):
    os.system("foamListTimes -case "+case+" -rm")
    os.system('rm -rf '+ case+'/0 '+case+'/log.* ')
    os.system('cp -r '+ case+'/0.orig '+ case+'/0')
else:
   print "!!!!! The answer is ",answer," and not accepted !!!!!"
   exit()

latesTime=0.
 
##########################################################################################
##########################################################################################

if (startFrom=="firstTime"):
    

    blockMeshDictOrig=case+"/system/blockMeshDict.orig"
    blockMeshDict=case+"/system/blockMeshDict"

    os.system('sed -e s/LconvertToMeters/'+str(1.)+'/g  '+blockMeshDictOrig+' > '+blockMeshDict)
    os.system('sed -i s/NxAir/'+str(airNx)+'/g  '+blockMeshDict)
    os.system('sed -i s/NyAir/'+str(airNy)+'/g  '+blockMeshDict)
    os.system('sed -i s/NxPCM/'+str(pcmNx)+'/g  '+blockMeshDict)
    os.system('sed -i s/NyPCM/'+str(pcmNy)+'/g  '+blockMeshDict)
    os.system('sed -i s/XMINaIR/'+str(XMINaIR)+'/g  '+blockMeshDict)
    os.system('sed -i s/XMAXaIR/'+str(XMAXaIR)+'/g  '+blockMeshDict)
    os.system('sed -i s/XMAXpCM/'+str(XMAXpCM)+'/g  '+blockMeshDict)
    os.system('sed -i s/YMINaIR/'+str(YMINaIR)+'/g  '+blockMeshDict)
    os.system('sed -i s/YMAXaIR/'+str(YMAXaIR)+'/g  '+blockMeshDict)
    os.system('sed -i s/YMINpCM/'+str(YMINpCM)+'/g  '+blockMeshDict)
    os.system('sed -i s/YMAXpCM/'+str(YMAXpCM)+'/g  '+blockMeshDict)
    os.system('sed -i s/ZMIN/'+str(ZMIN)+'/g  '+blockMeshDict)
    os.system('sed -i s/ZMAX/'+str(ZMAX)+'/g  '+blockMeshDict)

    os.system('echo running blockMesh...')
    os.system('blockMesh -case '+ case+' > '+case+'/log.blockMesh')
    os.system('echo running splitMeshRegions...')
    os.system('splitMeshRegions -cellZonesOnly -overwrite -case '+ case+' > '+case+'/log.splitMeshRegions')
    ##########################################################################################

    T0Orig=case+"/system/controlDict.orig"
    T0=case+"/system/controlDict"
    T=case+"/0/pcm"

    os.system('sed -i s/TGradRightPcm/'+str(TGradRightPcm)+'/g  '+T0)
    #!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

controlDictOrig=case+"/system/controlDict.orig"
controlDict=case+"/system/controlDict"
os.system('sed -e s/DELTAT/'+str(DELTAT)+'/g  '+controlDictOrig+' > '+controlDict)
os.system('sed -i s/STARTFROM/'+startFrom+'/g  '+controlDict)
os.system('sed -i s/ENDTIME/'+str(ENDTIME)+'/g  '+controlDict)
os.system('sed -i s/WRITEINTERVAL/'+str(WRITEINTERVAL)+'/g  '+controlDict)
##########################################################################################

##########################################################################################

transportPropertiesPCMOrig=case+"/constant/pcm/transportProperties.orig"
transportPropertiesPCM=case+"/constant/pcm/transportProperties"
os.system('sed -e s/rhoSOLID/'+str(pcmSolRho)+'/g  '+transportPropertiesPCMOrig+' > '+transportPropertiesPCM)
os.system('sed -i s/cpSOLID/'+str(pcmSolCp)+'/g  '+transportPropertiesPCM)
os.system('sed -i s/LSOLID/'+str(pcmLatentHeat)+'/g  '+transportPropertiesPCM)
os.system('sed -i s/kSOLID/'+str(pcmSolK)+'/g  '+transportPropertiesPCM)
os.system('sed -i s/TSOLID/'+str(pcmSolT)+'/g  '+transportPropertiesPCM)

os.system('sed -i s/nuLIQUID/'+str(pcmLiqNu)+'/g  '+transportPropertiesPCM)
os.system('sed -i s/rhoLIQUID/'+str(pcmLiqRho)+'/g  '+transportPropertiesPCM)
os.system('sed -i s/cpLIQUID/'+str(pcmLiqCp)+'/g  '+transportPropertiesPCM)
os.system('sed -i s/kLIQUID/'+str(pcmLiqK)+'/g  '+transportPropertiesPCM)
os.system('sed -i s/TLIQUID/'+str(pcmLiqT)+'/g  '+transportPropertiesPCM)
##########################################################################################


thermophysicalPropertiesAIROrig=case+"/constant/air/thermophysicalProperties.orig"
thermophysicalPropertiesAIR=case+"/constant/air/thermophysicalProperties"
os.system('sed -e s/MOLWEIGHT/'+str(airMolWeight)+'/g  '+thermophysicalPropertiesAIROrig+' > '+thermophysicalPropertiesAIR)
os.system('sed -i s/RHO/'+str(airRho)+'/g  '+thermophysicalPropertiesAIR)
os.system('sed -i s/CP/'+str(airCp)+'/g  '+thermophysicalPropertiesAIR)
os.system('sed -i s/MU/'+str(airMu)+'/g  '+thermophysicalPropertiesAIR)
os.system('sed -i s/PR/'+str(airPR)+'/g  '+thermophysicalPropertiesAIR)

##########################################################################################

decomposeParDictOrig=case+"/system/decomposeParDict.orig"
decomposeParDict=case+"/system/decomposeParDict"
os.system('sed -e s/NPROCESSOR/'+str(NProcessor)+'/g  '+decomposeParDictOrig+' > '+decomposeParDict)

##########################################################################################

if (startFrom=="firstTime"):

    os.system('echo running decomposePar -force...')
    os.system('decomposePar -case '+ case+' -force > '+case+'/log.decomposePar')

print 'The case is ready, you can run it by: \n'+'mpirun -np '+str(NProcessor)+' pcmFoam -parallel -case '+ case+' >& '+case+'/log.pcmFoam'
# ~ os.system('mpirun -np '+str(NProcessor)+' IBDRPimpleFoam -parallel -case '+ case+' >& '+case+'/log.IBDRPimpleFoam')
