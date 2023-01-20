# Bayan
## Overview
Bayan is a command-line utility for finding duplicate files.

## Parameters
```
--help,-h           Print help information.

--include-path,-i   One or more paths to be scanned. If not specified, the scan starts in the current directory.

                    Example:
                    -i /absolute/path ../relative/path

--exclude-path,-e   One or more paths which must not be scanned.

                    Example:
                    -e /absolute/path ../relative/path

--level,-l          A subdirectory level. E.g. 0 (default value) means that only the current directory must be scanned. 1 - the current directory and its subdirectories. And so on.

--min-file-size,-s  A minimum file size. Files which are smaller than the specified size will be skipped. The default value is 2.

                    Examples:

                    1 - one byte
                    1k - one kibibyte
                    1m - one megibyte
                    1g - one gibibyte

--file-name,-n      File name pattern. 
                    
                    Special characters:

                    *   Any zero or more characters.
                    ?   Any one character.
                    \   Removes special meaning of '?'. E.g.: \? means 'a question mark' (not a wildcard).

--block-size,-S     A number of bytes which are read per one I/O operation. The default value is 8.

                    Examples:

                    1 - one byte
                    1k - one kibibyte
                    1m - one megibyte
                    1g - one gibibyte

--hash-algorithm,-H A hash algorithm. Supported values are: crc32 (default) and md5.
```
