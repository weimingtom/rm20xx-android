#ifndef _SINGLETON_H__
#define _SINGLETON_H__

#include <assert.h>

#if OGRE_COMPILER == OGRE_COMPILER_MSVC
// Turn off warnings generated by this singleton implementation
#   pragma warning (disable : 4311)
#   pragma warning (disable : 4312)
#endif

#if defined ( OGRE_GCC_VISIBILITY )
#   pragma GCC visibility push(default)
#endif
namespace Ogre {
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup General
	*  @{
	*/

// End SJS additions
    /** Template class for creating single-instance global classes.
    */
    template <typename T> class Singleton
    {
	private:
		/** \brief Explicit private copy constructor. This is a forbidden operation.*/
		Singleton(const Singleton<T> &);

		/** \brief Private operator= . This is a forbidden operation. */
		Singleton& operator=(const Singleton<T> &);
    
	protected:

        static T* ms_Singleton;

    public:
        Singleton( void )
        {
            assert( !ms_Singleton );
#if defined( _MSC_VER ) && _MSC_VER < 1200	 
            int offset = (int)(T*)1 - (int)(Singleton <T>*)(T*)1;
            ms_Singleton = (T*)((int)this + offset);
#else
	    ms_Singleton = static_cast< T* >( this );
#endif
        }
        ~Singleton( void )
            {  assert( ms_Singleton );  ms_Singleton = 0;  }
        static T& getSingleton( void )
		{	assert( ms_Singleton );  return ( *ms_Singleton ); }
        static T* getSingletonPtr( void )
		{ return ms_Singleton; }
    };
	/** @} */
	/** @} */

}
#if defined ( OGRE_GCC_VISIBILITY )
#   pragma GCC visibility pop
#endif

#endif