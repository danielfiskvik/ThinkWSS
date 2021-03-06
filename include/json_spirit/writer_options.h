#ifndef WRITER_OPTIONS_H
#define	WRITER_OPTIONS_H

//          Copyright John W. Wilkinson 2007 - 2011
// Distributed under the MIT License, see accompanying file LICENSE.txt

// json spirit version 4.05

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace json_spirit {

    enum Output_options {
        pretty_print = 0x01, // Add whitespace to format the output nicely.

        raw_utf8 = 0x02, // This prevents non-printable characters from being escapted using "\uNNNN" notation.
        // Note, this is an extension to the JSON standard. It disables the escaping of
        // non-printable characters allowing UTF-8 sequences held in 8 bit char strings
        // to pass through unaltered.

        remove_trailing_zeros = 0x04,
        // NOTE: this is no longer meaningful, but used to output e.g. "1.200000000000000" as "1.2"
        single_line_arrays = 0x08,
        // pretty printing except that arrays printed on single lines unless they contain
        // composite elements, i.e. objects or arrays
    };
}

#endif