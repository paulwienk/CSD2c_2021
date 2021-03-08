if(NOT EXISTS "D:/hku/jaar2/CSD2c_2021/portaudio/cmake-build-debug/_deps/port_audio-build/install_manifest.txt")
  message(FATAL_ERROR "Cannot find install manifest: D:/hku/jaar2/CSD2c_2021/portaudio/cmake-build-debug/_deps/port_audio-build/install_manifest.txt")
endif(NOT EXISTS "D:/hku/jaar2/CSD2c_2021/portaudio/cmake-build-debug/_deps/port_audio-build/install_manifest.txt")

file(READ "D:/hku/jaar2/CSD2c_2021/portaudio/cmake-build-debug/_deps/port_audio-build/install_manifest.txt" files)
string(REGEX REPLACE "\n" ";" files "${files}")
foreach(file ${files})
  message(STATUS "Uninstalling $ENV{DESTDIR}${file}")
  if(IS_SYMLINK "$ENV{DESTDIR}${file}" OR EXISTS "$ENV{DESTDIR}${file}")
    exec_program(
      "C:/Program Files/JetBrains/CLion 2020.3/bin/cmake/win/bin/cmake.exe" ARGS "-E remove \"$ENV{DESTDIR}${file}\""
      OUTPUT_VARIABLE rm_out
      RETURN_VALUE rm_retval
      )
    if(NOT "${rm_retval}" STREQUAL 0)
      message(FATAL_ERROR "Problem when removing $ENV{DESTDIR}${file}")
    endif(NOT "${rm_retval}" STREQUAL 0)
  else(IS_SYMLINK "$ENV{DESTDIR}${file}" OR EXISTS "$ENV{DESTDIR}${file}")
    message(STATUS "File $ENV{DESTDIR}${file} does not exist.")
  endif(IS_SYMLINK "$ENV{DESTDIR}${file}" OR EXISTS "$ENV{DESTDIR}${file}")
endforeach(file)