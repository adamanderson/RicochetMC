SEED=$RANDOM$RANDOM$$

if test -z "${_CONDOR_SCRATCH_DIR}"; then
export _CONDOR_SCRATCH_DIR=/tmp/$USERNAME/mytmpdir/
fi

mkdir -p ${_CONDOR_SCRATCH_DIR}
cd ${_CONDOR_SCRATCH_DIR}
cp $G4WORKDIR/bin/Linux-g++/RMC_GGSim ./
cp $G4WORKDIR/macros/run_CRY.mac ./
./RMC_GGSim run_CRY.mac 1  2>> /net/hisrv0001/home/spitzj/outlog_ricochet
mv myG4output.root myOutput_"$SEED".root

i=`tail -n 1 /net/hisrv0001/home/spitzj/batch_runlist_ricochet|awk -F'[_]' '{print $2}'`

if [ "$i" == "" ]
then
i=10000
fi

let ii=$i+1

mv myOutput_"$SEED".root myOutput_"$ii".root 
echo run_"$ii"_seed_"$SEED">>/net/hisrv0001/home/spitzj/batch_runlist_ricochet

scp ${_CONDOR_SCRATCH_DIR}/myOut* nudsk0001:/net/nudsk0001/d00/scratch/spitzj/ricochet_out  2>> /net/hisrv0001/home/spitzj/outlog_ricochet
rm -rf ${_CONDOR_SCRATCH_DIR}
rm ${_CONDOR_SCRATCH_DIR}/*