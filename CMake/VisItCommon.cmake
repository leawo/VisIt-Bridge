
MACRO(VISIT_INSTALL_TARGETS target)     
  if(NOT PV_INSTALL_NO_LIBRARIES)    
    #install(TARGETS ${target}
    #  RUNTIME DESTINATION ${PV_INSTALL_BIN_DIR} COMPONENT Runtime
    #  LIBRARY DESTINATION ${PV_INSTALL_LIB_DIR} COMPONENT Runtime
    #  ARCHIVE DESTINATION ${PV_INSTALL_LIB_DIR} COMPONENT Development)
  endif(NOT PV_INSTALL_NO_LIBRARIES)
  # Development
  if(NOT PV_INSTALL_NO_DEVELOPMENT)
    GLOB_INSTALL_DEVELOPMENT(
      ${CMAKE_CURRENT_SOURCE_DIR}
      ${PV_INSTALL_INCLUDE_DIR}
      "*.h;*.hxx;*.txx")     
    if(dynamicHeaders)
      INSTALL(
        FILES ${dynamicHeaders}
        DESTINATION ${PV_INSTALL_INCLUDE_DIR}
        COMPONENT Development)
    endif(dynamicHeaders)
  endif(NOT PV_INSTALL_NO_DEVELOPMENT) 
  
ENDMACRO(VISIT_INSTALL_TARGETS)

FUNCTION(ADD_PARALLEL_LIBRARY target)
    ADD_LIBRARY(${target} ${ARGN})
    IF(VISIT_PARALLEL_CXXFLAGS)
        SET(PAR_COMPILE_FLAGS "")
        FOREACH (X ${VISIT_PARALLEL_CXXFLAGS})
            SET(PAR_COMPILE_FLAGS "${PAR_COMPILE_FLAGS} ${X}")
        ENDFOREACH(X)
        SET_TARGET_PROPERTIES(${target} PROPERTIES
            COMPILE_FLAGS ${PAR_COMPILE_FLAGS}
        )
        IF(VISIT_PARALLEL_LINKER_FLAGS)
            SET(PAR_LINK_FLAGS "")
            FOREACH(X ${VISIT_PARALLEL_LINKER_FLAGS})
                SET(PAR_LINK_FLAGS "${PAR_LINK_FLAGS} ${X}")
            ENDFOREACH(X)
            SET_TARGET_PROPERTIES(${target} PROPERTIES
                LINK_FLAGS ${PAR_LINK_FLAGS}
            )
        ENDIF(VISIT_PARALLEL_LINKER_FLAGS)

        IF(VISIT_PARALLEL_RPATH)
            SET(PAR_RPATHS "")
            FOREACH(X ${CMAKE_INSTALL_RPATH})
                SET(PAR_RPATHS "${PAR_RPATHS} ${X}")
            ENDFOREACH(X)
            FOREACH(X ${VISIT_PARALLEL_RPATH})
                SET(PAR_RPATHS "${PAR_RPATHS} ${X}")
            ENDFOREACH(X)
            SET_TARGET_PROPERTIES(${target} PROPERTIES
                INSTALL_RPATH ${PAR_RPATHS}
            )
        ENDIF(VISIT_PARALLEL_RPATH)

        IF(NOT VISIT_NOLINK_MPI_WITH_LIBRARIES)
            TARGET_LINK_LIBRARIES(${target} ${VISIT_PARALLEL_LIBS})
        ENDIF(NOT VISIT_NOLINK_MPI_WITH_LIBRARIES)
    ENDIF(VISIT_PARALLEL_CXXFLAGS)
ENDFUNCTION(ADD_PARALLEL_LIBRARY)

MACRO(VISIT_VTK_THIRD_PARTY_INCLUDE upper lower)
  if(VTK_USE_SYSTEM_${upper})
    if(${upper}_INCLUDE_DIR)
      include_directories(${${upper}_INCLUDE_DIR})
    endif(${upper}_INCLUDE_DIR)
  else(VTK_USE_SYSTEM_${upper})
    include_directories(
      ${VTK_BINARY_DIR}/Utilities/${lower}
      ${VTK_SOURCE_DIR}/Utilities/${lower}
    )
  endif(VTK_USE_SYSTEM_${upper})
ENDMACRO(VISIT_VTK_THIRD_PARTY_INCLUDE)

MACRO(VISIT_READER_INCLUDES)
  include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${vtkVisItAVTAlgorithms_BINARY_DIR}
    ${vtkVisItAVTAlgorithms_SOURCE_DIR}
    ${VISIT_COMMON_INCLUDES}
    ${VISIT_SOURCE_DIR}/include
    ${AVT_DATABASE_SOURCE_DIR}/Database
    ${AVT_DATABASE_SOURCE_DIR}/Ghost
    ${AVT_DATABASE_SOURCE_DIR}/Formats
    ${AVT_DBATTS_SOURCE_DIR}/MetaData
    ${AVT_DBATTS_SOURCE_DIR}/SIL
    ${VISIT_SOURCE_DIR}/avt/Math
    ${VISIT_SOURCE_DIR}/avt/VisWindow/VisWindow
    ${AVT_PIPELINE_SOURCE_DIR}/AbstractFilters
    ${AVT_PIPELINE_SOURCE_DIR}/Data
    ${AVT_PIPELINE_SOURCE_DIR}/Pipeline
    ${AVT_PIPELINE_SOURCE_DIR}/Sinks
    ${AVT_PIPELINE_SOURCE_DIR}/Sources
    ${VISIT_SOURCE_DIR}/visit_vtk/full
    ${VISIT_SOURCE_DIR}/visit_vtk/lightweight
    ${BOOST_INCLUDE_DIR}
    ${VTK_INCLUDE_DIRS}
    )
ENDMACRO(VISIT_READER_INCLUDES)

#called from readers that are being built into paraview
FUNCTION(ADD_VISIT_READER NAME VERSION)
  set(PLUGIN_NAME "vtk${NAME}")
  set(PLUGIN_VERSION "${VERSION}")
  set(ARG_VISIT_READER_NAME)
  set(ARG_VISIT_INCLUDE_NAME)
  set(ARG_VISIT_READER_TYPE)
  set(ARG_VISIT_READER_FILE_PATTERN)
  set(ARG_VISIT_READER_USES_OPTIONS OFF)
  set(ARG_SERVER_SOURCES)  

  PV_PLUGIN_PARSE_ARGUMENTS(ARG 
    "VISIT_READER_NAME;VISIT_INCLUDE_NAME;VISIT_READER_TYPE;VISIT_READER_FILE_PATTERN;VISIT_READER_USES_OPTIONS;SERVER_SOURCES"
      "" ${ARGN} )    
  #check reader types
  string(REGEX MATCH "^[SM]T[SM]D$" VALID_READER_TYPE ${ARG_VISIT_READER_TYPE})

  if ( NOT VALID_READER_TYPE)
    MESSAGE(FATAL_ERROR "Invalid Reader Type. Valid Types are STSD, STMD, MTSD, MTMD")
  endif()

  #if the user hasn't defined an include name, we presume the reader name
  #is also the include name
  if(NOT ARG_VISIT_INCLUDE_NAME)
    set(ARG_VISIT_INCLUDE_NAME ${ARG_VISIT_READER_NAME})
  endif()

  MESSAGE(STATUS "Generating wrappings for ${PLUGIN_NAME}")
  VISIT_READER_INCLUDES()

  if(ARG_VISIT_READER_USES_OPTIONS)
    #determine the name of the plugin info class by removing the 
    #avt from the start and the FileFormat from the back
    string(REGEX REPLACE "^avt|FileFormat$" "" TEMP_NAME ${ARG_VISIT_READER_NAME})
    set(ARG_VISIT_PLUGIN_INFO_HEADER ${TEMP_NAME}PluginInfo)
    set(ARG_VISIT_PLUGIN_INFO_CLASS ${TEMP_NAME}CommonPluginInfo)
  endif()

  set(XML_NAME ${NAME})
  #need to generate the VTK class wrapper
  string(SUBSTRING ${ARG_VISIT_READER_TYPE} 0 2 READER_WRAPPER_TYPE)
  configure_file(
      ${VISIT_CMAKE_DIR}/VisItExport.h.in
      ${CMAKE_CURRENT_BINARY_DIR}/${PLUGIN_NAME}Export.h @ONLY)      
  configure_file(
      ${VISIT_CMAKE_DIR}/VisIt${READER_WRAPPER_TYPE}.h.in
      ${CMAKE_CURRENT_BINARY_DIR}/${PLUGIN_NAME}.h @ONLY)  
  configure_file(
      ${VISIT_CMAKE_DIR}/VisIt${READER_WRAPPER_TYPE}.cxx.in
      ${CMAKE_CURRENT_BINARY_DIR}/${PLUGIN_NAME}.cxx @ONLY)
  
  set(reader_sources
  ${CMAKE_CURRENT_BINARY_DIR}/${PLUGIN_NAME}.cxx
  ${CMAKE_CURRENT_BINARY_DIR}/${PLUGIN_NAME}.h
    )    
  
  add_library(${NAME} ${ARG_SERVER_SOURCES} ${reader_sources})  
ENDFUNCTION(ADD_VISIT_READER)

#Used for readers that are plugins for paraview
FUNCTION(ADD_VISIT_PLUGIN_READER NAME VERSION)
set(PLUGIN_NAME "vtk${NAME}")
set(PLUGIN_VERSION "${VERSION}")
set(ARG_VISIT_READER_NAME)
set(ARG_VISIT_INCLUDE_NAME)
set(ARG_VISIT_READER_TYPE)
set(ARG_VISIT_READER_USES_OPTIONS OFF)
set(ARG_SERVER_SOURCES)

PV_PLUGIN_PARSE_ARGUMENTS(ARG 
  "VISIT_READER_NAME;VISIT_INCLUDE_NAME;VISIT_READER_TYPE;VISIT_READER_USES_OPTIONS;SERVER_SOURCES"
    "" ${ARGN} )    
#check reader types
string(REGEX MATCH "^[SM]T[SM]D$" VALID_READER_TYPE ${ARG_VISIT_READER_TYPE})

if ( NOT VALID_READER_TYPE)
  MESSAGE(FATAL_ERROR "Invalid Reader Type. Valid Types are STSD, STMD, MTSD, MTMD")
endif()

#if the user hasn't defined an include name, we presume the reader name
#is also the include name
if(NOT ARG_VISIT_INCLUDE_NAME)
  set(ARG_VISIT_INCLUDE_NAME ${ARG_VISIT_READER_NAME})
endif()

MESSAGE(STATUS "Generating wrappings for ${PLUGIN_NAME}")
VISIT_READER_INCLUDES()

if(ARG_VISIT_READER_USES_OPTIONS)
  #determine the name of the plugin info class by removing the 
  #avt from the start and the FileFormat from the back
  string(REGEX REPLACE "^avt|FileFormat$" "" TEMP_NAME ${ARG_VISIT_READER_NAME})
  set(ARG_VISIT_PLUGIN_INFO_HEADER ${TEMP_NAME}PluginInfo)
  set(ARG_VISIT_PLUGIN_INFO_CLASS ${TEMP_NAME}CommonPluginInfo)
endif()

set(XML_NAME ${NAME})
#need to generate the VTK class wrapper
string(SUBSTRING ${ARG_VISIT_READER_TYPE} 0 2 READER_WRAPPER_TYPE)
configure_file(
    ${VISIT_CMAKE_DIR}/VisItExport.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PLUGIN_NAME}Export.h @ONLY)      
configure_file(
    ${VISIT_CMAKE_DIR}/VisIt${READER_WRAPPER_TYPE}.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PLUGIN_NAME}.h @ONLY)  
configure_file(
    ${VISIT_CMAKE_DIR}/VisIt${READER_WRAPPER_TYPE}.cxx.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PLUGIN_NAME}.cxx @ONLY)
  
#generate server manager xml file  
configure_file(
  ${VISIT_CMAKE_DIR}/VisItSM.xml.in
  ${CMAKE_CURRENT_BINARY_DIR}/${PLUGIN_NAME}SM.xml @ONLY)

#generate reader xml 
configure_file(
  ${VISIT_CMAKE_DIR}/VisItGUI.xml.in
  ${CMAKE_CURRENT_BINARY_DIR}/${PLUGIN_NAME}GUI.xml @ONLY)  
  
  
set(reader_sources
  ${CMAKE_CURRENT_BINARY_DIR}/${PLUGIN_NAME}.cxx
  ${CMAKE_CURRENT_BINARY_DIR}/${PLUGIN_NAME}.h
    )  
set(reader_server_xml
  ${CMAKE_CURRENT_BINARY_DIR}/${PLUGIN_NAME}SM.xml
  )
set(reader_client_xml
  ${CMAKE_CURRENT_BINARY_DIR}/${PLUGIN_NAME}GUI.xml
  )

#add the vtk classes to the argument list
set(PV_ARGS ${ARGN})
list(APPEND PV_ARGS "SERVER_MANAGER_SOURCES;${reader_sources}")

#now we need to add the XML info
list(APPEND PV_ARGS "SERVER_MANAGER_XML;${reader_server_xml}")
list(APPEND PV_ARGS "GUI_RESOURCE_FILES;${reader_client_xml}")

ADD_PARAVIEW_PLUGIN( ${NAME} ${VERSION} ${PV_ARGS} )
ENDFUNCTION(ADD_VISIT_PLUGIN_READER)

FUNCTION(ADD_VISIT_INTERFACE_PLUGIN_READER NAME VERSION)

set(INTERFACE_NAME "vtk${NAME}")
set(INTERFACE_VERSION "${VERSION}")
set(ARG_VISIT_READER_NAMES)
set(ARG_VISIT_READER_TYPES)
set(ARG_VISIT_READER_INCLUDES)
set(ARG_VISIT_INTERFACE_CALL)
set(ARG_VISIT_INTERFACE_FILE)
set(ARG_VISIT_INTERFACE_EXEMPT_CLASSES)
set(ARG_VISIT_READER_FILE_PATTERN)
set(ARG_SERVER_SOURCES)
    
PV_PLUGIN_PARSE_ARGUMENTS(ARG 
  "VISIT_READER_NAMES;VISIT_READER_TYPES;VISIT_READER_INCLUDES;VISIT_INTERFACE_CALL;VISIT_INTERFACE_FILE;VISIT_INTERFACE_EXEMPT_CLASSES;VISIT_READER_FILE_PATTERN;SERVER_SOURCES"
    "" ${ARGN} )   
    
if ( NOT ARG_VISIT_INTERFACE_CALL OR NOT ARG_VISIT_INTERFACE_FILE )
  MESSAGE(FATAL_ERROR "The macro file for the file interface needs to be defined.")
endif()

    
message(STATUS "Generating wrappings for ${INTERFACE_NAME}")    
VISIT_READER_INCLUDES()    

#check reader types
set(INTERFACE_SOURCES)
set(INTERFACE_SMXML)
set(INTERFACE_GUIXML)
list(LENGTH ARG_VISIT_READER_NAMES NUM_READERS)
foreach( index RANGE ${NUM_READERS})
  if ( index LESS NUM_READERS )
    list(GET ARG_VISIT_READER_NAMES ${index} ARG_VISIT_READER_NAME)
    list(GET ARG_VISIT_READER_TYPES ${index} ARG_VISIT_READER_TYPE)
    list(GET ARG_VISIT_READER_INCLUDES ${index} ARG_VISIT_INCLUDE_NAME)
        
    #need to set up the vars needed by the configures
    string(REGEX REPLACE "^avt|FileFormat$" "" TEMP_NAME ${ARG_VISIT_READER_NAME})
    set(PLUGIN_NAME "vtk${TEMP_NAME}Reader")
    set(XML_NAME "VisIt${TEMP_NAME}Reader")
            
    #need to generate the VTK class wrapper
    string(SUBSTRING ${ARG_VISIT_READER_TYPE} 0 2 READER_WRAPPER_TYPE)
    
    #determine if this file is exempt from the interface CanReadFile macro    
    list(FIND ARG_VISIT_INTERFACE_EXEMPT_CLASSES ${ARG_VISIT_READER_NAME} EXEMPT_READER)
    if ( EXEMPT_READER EQUAL -1 )
      set(VISIT_READER_USES_INTERFACE ON)
    else()
      set(VISIT_READER_USES_INTERFACE OFF)    
    endif()    
    
    #we have to configure the macro file 
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/${ARG_VISIT_INTERFACE_FILE}.h
        ${CMAKE_CURRENT_BINARY_DIR}/${PLUGIN_NAME}${ARG_VISIT_INTERFACE_FILE}.h @ONLY)  
    
    #configure the declspec header
    configure_file(
        ${VISIT_CMAKE_DIR}/VisItExport.h.in
        ${CMAKE_CURRENT_BINARY_DIR}/${PLUGIN_NAME}Export.h @ONLY)      
    
    #configure the header and implementation
    configure_file(
        ${VISIT_CMAKE_DIR}/VisIt${READER_WRAPPER_TYPE}.h.in
        ${CMAKE_CURRENT_BINARY_DIR}/${PLUGIN_NAME}.h @ONLY)  
    configure_file(
        ${VISIT_CMAKE_DIR}/VisIt${READER_WRAPPER_TYPE}.cxx.in
        ${CMAKE_CURRENT_BINARY_DIR}/${PLUGIN_NAME}.cxx @ONLY)
        
      
      
    #generate server manager xml file  
    configure_file(
      ${VISIT_CMAKE_DIR}/VisItSM.xml.in
      ${CMAKE_CURRENT_BINARY_DIR}/${PLUGIN_NAME}SM.xml @ONLY)

    #generate reader xml 
    configure_file(
      ${VISIT_CMAKE_DIR}/VisItGUI.xml.in
      ${CMAKE_CURRENT_BINARY_DIR}/${PLUGIN_NAME}GUI.xml @ONLY)  
  
    LIST(APPEND INTERFACE_SOURCES 
      ${CMAKE_CURRENT_BINARY_DIR}/${PLUGIN_NAME}.cxx
      ${CMAKE_CURRENT_BINARY_DIR}/${PLUGIN_NAME}.h
      )
    LIST(APPEND INTERFACE_SMXML 
      ${CMAKE_CURRENT_BINARY_DIR}/${PLUGIN_NAME}SM.xml
      )
    LIST(APPEND INTERFACE_GUIXML 
      ${CMAKE_CURRENT_BINARY_DIR}/${PLUGIN_NAME}GUI.xml
      )
    
  endif()
endforeach( index )    

#add the vtk classes to the argument list
set(PV_ARGS ${ARGN})
list(APPEND PV_ARGS "SERVER_MANAGER_SOURCES;${INTERFACE_SOURCES}")

#now we need to add the XML info
list(APPEND PV_ARGS "SERVER_MANAGER_XML;${INTERFACE_SMXML}")
list(APPEND PV_ARGS "GUI_RESOURCE_FILES;${INTERFACE_GUIXML}")

ADD_PARAVIEW_PLUGIN( ${NAME} ${VERSION} ${PV_ARGS} )

ENDFUNCTION(ADD_VISIT_INTERFACE_PLUGIN_READER)


FUNCTION(VISIT_INCLUDE_READER_SOURCE name)
option(VISIT_BUILD_PLUGIN_${name} "Build VisIt reader {name} plugin" ON)
if (VISIT_BUILD_PLUGIN_${name})
  ADD_SUBDIRECTORY(${name})
endif()
ENDFUNCTION(VISIT_INCLUDE_READER_SOURCE name)