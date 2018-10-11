# File reading
#from glob import glob
#import AthenaPoolCnvSvc.ReadAthenaPool

runGrid = True

if runGrid:
  import AthenaRootComps.ReadAthenaxAOD
  svcMgr.EventSelector.AccessMode = 1

if not runGrid:
  #inputFile = 'stefano_bfield_RunI_files.txt'
  inputFile = 'test.txt'
  crimefile = open(inputFile, 'r')
  inputFiles = [f[0:len(f)-1] for f in crimefile]
  print(inputFiles[0])
  #override next line on command line with: --filesInput=XXX
  jps.AthenaCommonFlags.FilesInput = inputFiles 
  #Specify AccessMode (read mode) ... ClassAccess is good default for xAOD
  jps.AthenaCommonFlags.AccessMode = "ClassAccess" 

jps.AthenaCommonFlags.HistOutputs = ["ANALYSIS:bfield_map_2016.outputs.root"]
#jps.AthenaCommonFlags.HistOutputs = ["ANALYSIS:bfieldmap_RunI.outputs.root"]
#jps.AthenaCommonFlags.HistOutputs = ["ANALYSIS:test.root"]
svcMgr.THistSvc.MaxFileSize=-1 # speeds up jobs that output lots of histograms?

# create our algorithm with the given name
#alg = CfgMgr.MuonAODAnalysis()
alg = CfgMgr.MuonSelection()

# later on we'll add some configuration options for our algorithm that go here

#Add our algorithm to the main alg sequence
athAlgSeq += alg

# optional include for reducing printout from athena
include("AthAnalysisBaseComps/SuppressLogging.py")
