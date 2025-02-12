add_test( random /home/vacko/ipk2/ipk2025/build/tests/tests random  )
set_tests_properties( random PROPERTIES WORKING_DIRECTORY /home/vacko/ipk2/ipk2025/build/tests SKIP_RETURN_CODE 4)
set( tests_TESTS random)
