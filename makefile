# Nome do binário gerado
TARGET = parse_json.out

# Compiler e flags
CXX = g++
CXXFLAGS = -std=c++17 -O3

# Arquivos do projeto
SRCS = main.cpp simdjson.cpp parser.cpp
OBJS = $(SRCS:.cpp=.o)

# Regra principal
all: $(TARGET)

# Regra para compilar o executável
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Regra para compilar os arquivos .cpp
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpeza
clean:
	rm -f $(TARGET) $(OBJS)
