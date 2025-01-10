# Nome do binário gerado
TARGET = parse_json.out
PROFILE_TARGET = parse_json_profile.out

# Compiler e flags
CXX = g++
CXXFLAGS = -std=c++17 -O3
PROFILE_FLAGS = -pg

# Arquivos do projeto
SRCS = main.cpp simdjson.cpp parser.cpp
OBJS = $(SRCS:.cpp=.o)
PROFILE_OBJS = $(SRCS:.cpp=.profile.o)

# Regra principal
all: $(TARGET)

# Regra para compilar o executável
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Regra para compilar o executável com profiling
profile: $(PROFILE_TARGET)

$(PROFILE_TARGET): $(PROFILE_OBJS)
	$(CXX) $(CXXFLAGS) $(PROFILE_FLAGS) -o $@ $^

# Regras para compilar os arquivos .cpp
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.profile.o: %.cpp
	$(CXX) $(CXXFLAGS) $(PROFILE_FLAGS) -c $< -o $@

# Limpeza
clean:
	rm -f $(TARGET) $(PROFILE_TARGET) $(OBJS) $(PROFILE_OBJS) gmon.out
