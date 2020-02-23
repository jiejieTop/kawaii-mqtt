Import('RTT_ROOT')
from building import *

# get current directory
cwd = GetCurrentDir()

# The set of source files associated with this SConscript file.
src = Glob('common/*.c')
src += Glob('mqtt/*.c')
src += Glob('mqttclient/*.c')
src += Glob('network/*.c')
src += Glob('platform/RT-Thread/*.c')

if GetDepend(['PKG_USING_MQTTCLIENT_TEST']):
    src += Glob('tests/*.c')

path = [cwd + '/common']
path += [cwd + '/mqtt']
path += [cwd + '/mqttclient']
path += [cwd + '/network']
path += [cwd + '/platform/RT-Thread']

group = DefineGroup('mqttclient', src, depend = ['PKG_USING_MQTTCLIENT'], CPPPATH = path)


Return('group')
