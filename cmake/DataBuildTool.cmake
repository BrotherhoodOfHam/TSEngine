#
#   Data Builder tool
#

include(PythonUtils)

set(DATABUILD_INPUT_DIR  "${PROJECT_SOURCE_DIR}/data")
set(DATABUILD_OUTPUT_DIR "${PROJECT_BINARY_DIR}/data")
set(DATABUILD_TOOL "${PYWRAPPER_COMMAND}" "${PROJECT_SOURCE_DIR}/tools/dbuild.py")

file(MAKE_DIRECTORY ${DATABUILD_OUTPUT_DIR})

install(
    DIRECTORY "${DATABUILD_OUTPUT_DIR}/"
    DESTINATION "data"
    PATTERN ".ninja_deps" EXCLUDE
    PATTERN ".ninja_log" EXCLUDE
    PATTERN "build.ninja" EXCLUDE
    PATTERN "index.dat" EXCLUDE
)

add_custom_target(DATA_BUILD ALL
	WORKING_DIRECTORY ${DATABUILD_OUTPUT_DIR}
    COMMAND ${DATABUILD_TOOL} "--build" "${DATABUILD_INPUT_DIR}"
)

set_target_properties(
	DATA_BUILD
	PROPERTIES FOLDER commands
)
