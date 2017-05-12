# Copyright (c) 2017 Ivan Vaigult
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

function(add_yatl_test test_name)
  set(test_status_file "${CMAKE_CURRENT_BINARY_DIR}/${test_name}.ok")
  add_custom_command(OUTPUT ${test_status_file}
            COMMAND $<TARGET_FILE:yatl> -p ${CMAKE_CURRENT_SOURCE_DIR}/test-framework.scm ${CMAKE_CURRENT_SOURCE_DIR}/${test_name}.scm
			COMMAND ${CMAKE_COMMAND} -E touch ${test_status_file}
			DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/test-framework.scm ${CMAKE_CURRENT_SOURCE_DIR}/${test_name}.scm
  )
  add_custom_target(launch_${test_name} DEPENDS ${test_status_file})
  set_target_properties(launch_${test_name} PROPERTIES FOLDER "tests")
  if (NOT TARGET run_all_tests)
    add_custom_target(run_all_tests)
  endif()
  add_dependencies(run_all_tests launch_${test_name})
endfunction()


add_yatl_test(list_test)
add_yatl_test(types_test)