./dmalloc_test > dmalloc_test.1
diff dmalloc_test.correct dmalloc_test.1
rm dmalloc_test.1

./lisp_test.exe > lisp_test.1
diff lisp_test.correct lisp_test.1
rm lisp_test.1

./logging_test.exe > logging_test.1
diff logging_test.correct logging_test.1
rm logging_test.1

./strbuf_test.exe > strbuf_test.1
diff strbuf_test.correct strbuf_test.1
rm strbuf_test.1

./string_list_test.exe > string_list_test.1
diff string_list_test.correct string_list_test.1
rm string_list_test.1

./test-regex.exe > test-regex.1
diff test-regex.correct test-regex.1
rm test-regex.1

./testrbtree > testrbtree.test
diff testrbtree.test.correct testrbtree.test
rm testrbtree.test

./stuff_test > stuff_test.1
diff stuff_test.1 stuff_test.correct
rm stuff_test.1

./dlist_test > dlist_test.1
diff dlist_test.1 dlist_test.correct
rm dlist_test.1
