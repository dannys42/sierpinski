import commands
import SCons.Environment
import SCons.MultiEnv

default_path = '/usr/local/bin:/bin:/usr/bin'
global_CCFLAGS = [ '' ];

def MakeShared(env):
    env['LIBRARY_TYPE'] = 'shared'
    return(env)

def MakeStatic(env):
    env['LIBRARY_TYPE'] = 'static'
    return(env)


def AppEnvironment(menv = None):
    if menv == None:
        menv = SCons.MultiEnv.MultiEnv()

    # Common definitions to all builds
    common = SCons.Environment.Environment()
    common['ENV']['PATH'] = default_path
    common['CC'] = 'gcc'
    common['LINK'] = 'g++'
    common['CCFLAGS'] = [ '-Wall', '-pedantic' ] + global_CCFLAGS
    common['CPPFLAGS'] = [ ]
    common['LINKFLAGS'] = [ ]
    common['LIBPATH'] = [ ]

    # Common Desktop definitions
    desktop = common.Copy()
    desktop.Append( CPPPATH=['/usr/include/libxml2'] )

    # Desktop Optimized
    desktop_optimized = desktop.Copy()
    desktop_optimized.Append(CCFLAGS='-O6')

    desktop_optimized_static = MakeStatic( desktop_optimized.Copy() )
    desktop_optimized_shared = MakeShared( desktop_optimized.Copy() )

    menv.AddEnvironment('desktop-optimized-static', desktop_optimized_static)
    menv.AddEnvironment('desktop-optimized-shared', desktop_optimized_shared)

    # Desktop DMALLOC (32-bit)
    desktop_dmalloc = desktop.Copy()
    desktop_dmalloc['CC'] = 'gcc32'
    desktop_dmalloc['LINK'] = 'g++32'
    desktop_dmalloc.Append(
        CCFLAGS = [ '-g', '-DDMALLOC', '-DDMALLOC_FUNC_CHECK', '-m32', '-march=i386'],
        LIBS = ['dmalloc'],
        LINKFLAGS = [ '-m32' ],
        )

    desktop_dmalloc_static = MakeStatic( desktop_dmalloc.Copy() )
    desktop_dmalloc_shared = MakeShared( desktop_dmalloc.Copy() )

    menv.AddEnvironment('desktop-dmalloc-static', desktop_dmalloc_static);
    menv.AddEnvironment('desktop-dmalloc-shared', desktop_dmalloc_shared);

    # Desktop Profile
    desktop_profile = desktop.Copy()
    desktop_profile.Append(
        CCFLAGS = [ '-g', '-pg' ],
        LIINKFLAGS = [ '-pg' ]
        )
    desktop_profile_static = MakeStatic( desktop_profile.Copy() )
    desktop_profile_shared = MakeShared( desktop_profile.Copy() )

    menv.AddEnvironment('destop-profile-static', desktop_profile_static)
    menv.AddEnvironment('destop-profile-shared', desktop_profile_shared)

    return(menv)

