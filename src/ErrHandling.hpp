
#ifndef _JAS_ERR_HANDLING_HPP_
#define _JAS_ERR_HANDLING_HPP_

#include <string>
#include <iostream>

// Verwijder/Zet de comment marks hierbeneden voor runtime debug opties.
#define ___DBUG

#ifndef ___DBUG
#define ASSERT(x)
#else
#define ASSERT(x) \
		if (! (x)) \
			{ \
				std::cout << "ERROR!! Assert " << #x << " failed\n"; \
		        std::cout << " on line " << __LINE__  << "\n"; \
		        std::cout << " in file " << __FILE__ << "\n";  \
		    }
#endif

#ifndef ___DBUG
#define PRINT(x)
#else
#define PRINT(x) \
 	  std::cout << #x << ":\t" << x << std::endl;
#endif

class ERR {
public:

    class SDL_Err {
    };

    class Out_Of_Memory {
    };
};

#endif


