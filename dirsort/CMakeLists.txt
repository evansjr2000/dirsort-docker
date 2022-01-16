project (DIRSORT)

cmake_minimum_required (VERSION 2.6)

add_executable ( dirsort dirsort.c dirsort.web )

target_link_libraries (dirsort /usr/local/lib/libmisc.a)


set (DOCS dirsort)
set (TANGLE ctangle)
set (WEAVE cweave)
set (PDFTEX pdftex)
#set (PRETTY astyle)
set (IMPORTED_LOCATION /Users/evansjr/src/lib)

foreach (DOC ${DOCS})

	add_custom_command (
	    OUTPUT ${DOC}.c
	    DEPENDS ${DOC}.web
	    COMMAND ${TANGLE}
	    ARGS ${DOC}.web
#	    COMMAND ${PRETTY}
	    ARGS ${DOC}.c
	    )

	add_custom_command (
	    OUTPUT ${DOC}.tex
	    DEPENDS ${DOC}.aweb
	    COMMAND ${AWEAVE} 
	    ARGS ${DOC}.aweb
	    )

	add_custom_command (
	    OUTPUT ${DOC}.pdf
	    DEPENDS ${DOC}.tex
	    COMMAND ${PDFTEX}
	    ARGS ${DOC}.tex
	    )



# Build a list of all the results
  	set (DOC_RESULTS ${DOC_RESULTS}
	    ${DOC}
	    )

endforeach (DOC)


add_custom_target( TDocument ALL
		   DEPENDS ${DOC_RESULTS}
		   )




