env = Environment()
env.Append(CPPFLAGS=['-I./include/', '-ggdb'])
env.ParseConfig('pkg-config nlohmann_json --cflags')
env.Program('rpgpp', Glob('src/*.cpp'), LIBS=['m', 'libraylib'])
