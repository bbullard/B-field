# The name of the package:
atlas_subdir (MuonAnalysis)

# Add the shared library:
atlas_add_library (MuonAnalysisLib
  MuonAnalysis/*.h Root/*.cxx
  PUBLIC_HEADERS MuonAnalysis
  LINK_LIBRARIES AnaAlgorithmLib xAODEventInfo 
                 xAODMuon xAODTruth xAODTracking 
								 MuonAnalysisInterfacesLib
                 AsgAnalysisInterfaces)

# Add the dictionary:
atlas_add_dictionary (MuonAnalysisDict
  MuonAnalysis/MuonAnalysisDict.h
  MuonAnalysis/selection.xml
  LINK_LIBRARIES MuonAnalysisLib)

if (NOT XAOD_STANDALONE)
  # Add a component library for AthAnalysis only:
  atlas_add_component (MuonAnalysis
    src/components/*.cxx
    LINK_LIBRARIES MuonAnalysisLib)
endif ()

# Install files from the package:
atlas_install_joboptions( share/*.py )
