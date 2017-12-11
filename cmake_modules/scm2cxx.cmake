set(THIS_SCRIPT_DIR ${CMAKE_CURRENT_LIST_DIR})

function(scm2cxx tgt sources scm)
    get_filename_component(name_we "${scm}" NAME_WE)
    set(name_cxx "${CMAKE_CURRENT_BINARY_DIR}/${name_we}.cxx")
    set(name_inl "${CMAKE_CURRENT_BINARY_DIR}/${name_we}.inl")
    set(name_scm "${CMAKE_CURRENT_SOURCE_DIR}/${scm}")
    add_custom_command(OUTPUT ${name_inl}
	COMMAND $<TARGET_FILE:rc> ${name_scm} ${name_inl}
	DEPENDS $<TARGET_FILE:rc> ${name_scm})
    
    add_custom_target(generate_${name_we} DEPENDS ${name_inl})
    add_dependencies(${tgt} generate_${name_we})

    configure_file("${THIS_SCRIPT_DIR}/scm2cxx.in" "${name_cxx}")
    list(APPEND ${sources} ${name_cxx})
    set(${sources} ${${sources}} PARENT_SCOPE)
endfunction()

function(add_scm_library libname)
    add_custom_target(${libname}_scm2cxx)
    foreach(file ${ARGN})
	if (${file} MATCHES "^.*scm$")
	    scm2cxx(${libname}_scm2cxx ${libname}_sources ${file})
	else()
	    list(APPEND ${libname}_sources ${file})
	endif()
    endforeach()
    add_library(${libname} ${${libname}_sources})
    add_dependencies(${libname} ${libname}_scm2cxx)
endfunction()

