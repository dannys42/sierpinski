import commands
import os
import SCons.Environment
import SCons.MultiEnv

default_path = '/usr/local/bin:/bin:/usr/bin'
global_CCFLAGS = [ '' ];
arch = os.uname()[4]

def MakeShared(env):
    env['LIBRARY_TYPE'] = 'shared'
    return(env)

def MakeStatic(env):
    env['LIBRARY_TYPE'] = 'static'
    return(env)

def AppEnvironmentArch(menv=None, common=None, arch=None):
    # Common Desktop definitions
    desktop = common.Copy()
    desktop.Append( CPPPATH=['/usr/include/libxml2'] )

    # Desktop Optimized
    desktop_optimized = desktop.Copy()
    desktop_optimized.Append(CCFLAGS='-O6')

    desktop_optimized_static = MakeStatic( desktop_optimized.Copy() )
    desktop_optimized_shared = MakeShared( desktop_optimized.Copy() )

    menv.AddEnvironment('desktop-'+arch+'-optimized-static', desktop_optimized_static)
    menv.AddEnvironment('desktop-'+arch+'-optimized-shared', desktop_optimized_shared)

    # Desktop DMALLOC
    # At least on Fedora Core 5, this is not available for 64-bit
    if arch != 'x86_64':
        desktop_dmalloc = desktop.Copy()
        desktop_dmalloc.Replace(
            CC = 'gcc32',
            LINK = 'g++32',
            )
        desktop_dmalloc.Append(
            CCFLAGS = [ '-g', '-DDMALLOC', '-DDMALLOC_FUNC_CHECK', '-m32', '-march=i386'],
            LIBS = ['dmalloc'],
            LINKFLAGS = [ '-m32' ],
            )

        desktop_dmalloc_static = MakeStatic( desktop_dmalloc.Copy() )
        desktop_dmalloc_shared = MakeShared( desktop_dmalloc.Copy() )

        menv.AddEnvironment('desktop-'+arch+'-dmalloc-static', desktop_dmalloc_static);
        menv.AddEnvironment('desktop-'+arch+'-dmalloc-shared', desktop_dmalloc_shared);

    # Desktop Profile
    desktop_profile = desktop.Copy()
    desktop_profile.Append(
        CCFLAGS = [ '-g', '-pg' ],
        LINKFLAGS = [ '-pg' ]
        )
    desktop_profile_static = MakeStatic( desktop_profile.Copy() )
    desktop_profile_shared = MakeShared( desktop_profile.Copy() )

    menv.AddEnvironment('desktop-'+arch+'-profile-static', desktop_profile_static)
    menv.AddEnvironment('desktop-'+arch+'-profile-shared', desktop_profile_shared)

    return menv



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

    common32 = common.Copy()
    if arch == 'x86_64':
        # If we're runng on a 64-bit system, compile both 32-bit and
        # 64-bit versions
        common32.Replace(
            CC='gcc32',
            LINK='g++32',
        )
        AppEnvironmentArch(menv, common32, 'i386')
        AppEnvironmentArch(menv, common, arch)
    else:       # assume i386
        AppEnvironmentArch(menv, common, 'i386')

    return(menv)

