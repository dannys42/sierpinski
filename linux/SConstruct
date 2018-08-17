from SCons.MultiEnv import MultiEnv
from LocalEnvironment import AppEnvironment

amenv = AppEnvironment()
Export('amenv')

APP_DIRS = [
    'libsierp',
    'wxSierpinski',
]

amenv.BuildDir('#/build/<ENVIRONMENT>/<DIR>', APP_DIRS, duplicate=0)

Default('targets')
