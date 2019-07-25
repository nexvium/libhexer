#
# Copyright 2019 Javier Alvarado
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
CPPFLAGS := -DDEBUG -I..
CXXFLAGS := -ggdb3 -O0 -std=c++0x -pedantic-errors \
            -Wall -Wextra -Wshadow -Wparentheses -Wconversion
TARGET := libhexer

OBJDIR := objs
DEPDIR := deps

SOURCES := $(wildcard *.cpp)
OBJECTS := $(patsubst %.cpp,%.o,$(SOURCES))

$(TARGET): $(OBJECTS:%=$(OBJDIR)/%)
	$(CXX) $(CXXFLAGS) $(OBJECTS:%=$(OBJDIR)/%) -o $(TARGET)

$(DEPDIR)/%.d: %.cpp | $(DEPDIR)
	@set -e; rm -f $@; \
	 $(CXX) -MM $(CPPFLAGS) $< > $@.$$$$; \
	 sed 's,\($*\)\.o[ :]*,$(OBJDIR)/\1.o $@ : ,g' < $@.$$$$ > $@; \
	 rm -f $@.$$$$

.SECONDEXPANSION:
$(OBJECTS:%=$(OBJDIR)/%): $$(patsubst $(OBJDIR)/%.o,%.cpp,$$@) \
                          $$(patsubst $(OBJDIR)/%.o,$(DEPDIR)/%.d,$$@) \
                          | $(OBJDIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(OBJDIR) $(DEPDIR):
	mkdir -p $@

-include $(DEPDIR)/*.d

.PHONY: clean
clean:
	rm -rf $(DEPDIR) $(OBJDIR) $(TARGET)

.PHONY: debug
debug:
	@echo SOURCES=$(SOURCES)
	@echo OBJECTS=$(OBJECTS)
	@echo $(OBJECTS:%=$(OBJDIR)/%)

# eof
