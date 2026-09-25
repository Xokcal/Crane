cmake_minimum_required(VERSION 3.27)
project(craneC C)

set(CMAKE_C_STANDARD 11)

add_executable(craneC main.c
        tool/file/file.c
        tool/file/file.h
        tool/String/String.h
        tool/String/String.c
        cranegram/cgm_file.c
        cranegram/cgm_file.h
        tool/log/log.c
        tool/log/log.h
        cranegram/cgm_parse.c
        cranegram/cgm_parse.h
        cranegram/cgm_gramma.c
        cranegram/cgm_gramma.h
        tool/map/hashmap_int.c)
