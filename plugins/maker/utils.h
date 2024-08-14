#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

std::string proc(const char* cmd, int& exits, bool verbose=false);

size_t split_str(const std::string &str, char delim,
                        std::vector<std::string> &a);
#endif // UTILS_H
