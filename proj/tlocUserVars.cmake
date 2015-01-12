if(${SOLUTION_NAME} STREQUAL "solution")
  set(SOLUTION_NAME "solution_${SOLUTION_BUILD_FOLDER_NAME}" CACHE STRING "You may change the name based on build configurations" FORCE)
  project(${SOLUTION_NAME})
endif()

set(SOLUTION_EXECUTABLE_PROJECTS "skopworks;")
#list(APPEND USER_EXECUTABLE_PROJECTS "nextExeProject;")

set(SOLUTION_LIBRARY_PROJECTS "skopworksLib;")
#list(APPEND USER_LIBRARY_PROJECTS "nextLibraryProject;")

set(SOLUTION_TEST_PROJECTS "skopworksTest;")
#list(APPEND USER_TEST_PROJECTS "nextTestProject;")
