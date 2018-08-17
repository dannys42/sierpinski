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

def AppEnvironmentArch(menv=None, common=None, platform=None, arch=None):
    # Common Desktop definitions
    current = common.Copy()
    current.Append( CPPPATH=['/usr/include/libxml2'] )

    # Desktop Optimized
    current_optimized = current.Copy()
    current_optimized.Append(CCFLAGS='-O6')

    current_optimized_static = MakeStatic( current_optimized.Copy() )
    current_optimized_shared = MakeShared( current_optimized.Copy() )

    menv.AddEnvironment(platform+'-'+arch+'-optimized-static', current_optimized_static)
    menv.AddEnvironment(platform+'-'+arch+'-optimized-shared', current_optimized_shared)

    # Debug build
    current_debug = current.Copy()
    current_debug.Append(CCFLAGS='-g')

    current_debug_static = MakeStatic( current_debug.Copy() )
    current_debug_shared = MakeShared( current_debug.Copy() )

    menv.AddEnvironment(platform+'-'+arch+'-debug-static', current_debug_static)
    menv.AddEnvironment(platform+'-'+arch+'-debug-shared', current_debug_shared)

    # Desktop DMALLOC
    # At least on Fedora Core 5, this is not available for 64-bit
    current_dmalloc = current.Copy()
    current_dmalloc.Append(
        CCFLAGS = [ '-g', '-DDMALLOC', '-DDMALLOC_FUNC_CHECK' ],
        LIBS = ['dmalloc'],
        )

    current_dmalloc_static = MakeStatic( current_dmalloc.Copy() )
    current_dmalloc_shared = MakeShared( current_dmalloc.Copy() )

    menv.AddEnvironment(platform+'-'+arch+'-dmalloc-static', current_dmalloc_static);
    menv.AddEnvironment(platform+'-'+arch+'-dmalloc-shared', current_dmalloc_shared);

    # Desktop Profile
    current_profile = current.Copy()
    current_profile.Append(
        CCFLAGS = [ '-g', '-pg' ],
        LINKFLAGS = [ '-pg' ]
        )
    current_profile_static = MakeStatic( current_profile.Copy() )
    current_profile_shared = MakeShared( current_profile.Copy() )

    menv.AddEnvironment(platform+'-'+arch+'-profile-static', current_profile_static)
    menv.AddEnvironment(platform+'-'+arch+'-profile-shared', current_profile_shared)

    return menv



def AppEnvironment(menv = None):
    if menv == None:
        menv = SCons.MultiEnv.MultiEnv()

    # Common definitions to all builds
    common = SCons.Environment.Environment()
    common['ENV']['PATH'] = default_path
    common['CC'] = 'gcc'
    common['LINK'] = 'g++'
    common['CCFLAGS'] = [
        '-Wall',
        '-pedantic',
        '-Wno-long-long',  # to surpress g++ bug
        ] + global_CCFLAGS
    common['CPPFLAGS'] = [ ]
    common['LINKFLAGS'] = [ ]
    common['LIBPATH'] = [ ]

    # i386 build
    common_i386 = CommonEnvironment_i386(common)
    common_x86_64 = CommonEnvironment_Athlon64(common)

    AppEnvironmentArch(menv, common_i386, 'linux', 'i386')
    AppEnvironmentArch(menv, common_x86_64, 'linux', 'x86_64')

    return(menv)


def CommonEnvironment_i386(env):
    newenv = env.Copy()
    newenv.Append(
        CCFLAGS = [ '-m32', '-march=i386', '-mtune=i386' ],
        LINKFLAGS = [ '-m32' ],
        )
    return newenv;

def CommonEnvironment_Athlon64(env):
    newenv = env.Copy()
    newenv.Append(
        CCFLAGS = [ '-m64', '-march=athlon64', '-mtune=athlon64' ],
        LINKFLAGS = [ '-m64' ],
        )
    return newenv;

