env = Environment()
env.Append(CPPFLAGS=['-I./include/', '-ggdb'])
env.Program('rpgpp', Glob('src/*.cpp'), LIBS=['m', 'libraylib'])
