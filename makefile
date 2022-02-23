SOURCE_1=mandel
SOURCE_2=ioutil

TARGET=$(SOURCE_1)
MODULES=$(SOURCE_1).o $(SOURCE_2).o

CPPFLAGS=$(shell Magick++-config --cppflags) -g -ansi -Wall -Wconversion
LDFLAGS=$(shell Magick++-config --ldflags) -g

$(TARGET): $(MODULES)
	g++ $(LDFLAGS) $(MODULES) -o $(TARGET)

clean:
	rm -f $(TARGET) $(MODULES) *.o *~

all: clean $(TARGET)
	$(TARGET)

$(SOURCE_1).o: $(SOURCE_1).cpp
	g++ $(CPPFLAGS) -c $(SOURCE_1).cpp

$(SOURCE_2).o: $(SOURCE_2).cpp
	g++ $(CPPFLAGS) -c $(SOURCE_2).cpp
