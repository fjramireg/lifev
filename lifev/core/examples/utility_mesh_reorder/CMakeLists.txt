INCLUDE(TribitsAddExecutable)
INCLUDE(TribitsCopyFilesToBinaryDir)

TRIBITS_ADD_EXECUTABLE(
  MeshReorder
  SOURCES main.cpp
)

TRIBITS_COPY_FILES_TO_BINARY_DIR(data_utility_meshReorder
  CREATE_SYMLINK
  SOURCE_FILES data
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}
)

TRIBITS_COPY_FILES_TO_BINARY_DIR(mesh_utility_meshReorder
  CREATE_SYMLINK
  SOURCE_FILES tube20.mesh vessel20.mesh
  SOURCE_DIR ${CMAKE_SOURCE_DIR}/lifev/fsi/data/mesh/inria/
)
