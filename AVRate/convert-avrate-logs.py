#!/usr/bin/env python

'''
convert-avrate-logs.py

Author:   Werner Robitza
Synopsis: Converts AVRate logs into tidy data format.
Requirements: Python and d
Usage:    convert-avrate-logs.py -h
'''

import argparse
import datetime
import re
import os
import csv
from collections import OrderedDict

def main():
    '''
    Main function to be execute
    '''
    parser = argparse.ArgumentParser(description='AVRate Log Converter')
    parser.add_argument('-i', '--input', type=str, nargs='+', required=True, help='Input files to parse')
    parser.add_argument('-o', '--output', type=str, required=True, help='Output file for collecting data. Will be formatted as CSV.')
    parser.add_argument('-f', '--force',  action='store_true', help='Force overwriting existing output files')
    parser.add_argument('-e', '--header', action='store_true', help='Write CSV header to output file(s)')
    cli_args = parser.parse_args()

    data = []
    for f in cli_args.input:
        data.append(parse_file(f))

    all_data = [j for i in data for j in i] # props to: http://stackoverflow.com/a/716761/435093
    write_data(all_data, cli_args.output, force=cli_args.force, header=cli_args.header)

def write_data(data, file_path, force=False, header=False):
    """
    Write data to a file path
    """
    if not force and os.path.isfile(file_path):
        raise IOError('File exists, not overwriting. Use -f/--force to overwrite.')

    with open(file_path, 'w') as f:
        writer = csv.DictWriter(f, fieldnames = data[0].keys())
        if header:
            writer.writeheader()
        for line in data:
            writer.writerow(line)


def parse_file(input_file):
    '''
    Parse an input file and return the data as a list of ratings
    '''

    data = []

    with open(input_file, 'r') as f:
        lines = f.readlines()

        state = "STATE_GENERAL"

        playlist_id = None
        log_id      = None

        for line in lines:
            line = line.rstrip()
            if not len(line):
                continue

            if state == "STATE_GENERAL":
                if line.startswith("Playlist File:"):
                    playlist    = re.search(r'Playlist File:\s(.*)', line).group(1)
                    playlist_id = re.findall(r'(\d+)', playlist)[-1]
                    continue
                if line.startswith("ID:"):
                    log_id = re.search(r'ID:\s(.*)', line).group(1)
                    continue

                # continue section: training data
                if "- Results -" in line:
                    state = "STATE_RATINGS_TRAINING"
                    continue

                # continue section: real experiment data
                if ":TE" in line:
                    state = "STATE_RATINGS"
                    continue

            # collect ratings
            if state == "STATE_RATINGS_TRAINING" or state == "STATE_RATINGS":
                if line.startswith(':'):
                    continue

                if "Rating Histogram" in line:
                    state = "STATE_RATING_HISTOGRAM"
                    continue

                fields = line.split(' ')
                datetime_parsed = datetime.datetime.strptime(fields[0] + ' ' + fields[1], '%d.%m.%Y %H:%M:%S')
                datetime_f      = datetime_parsed.strftime("%Y-%m-%dT%H:%M:%S") # format in ISO 8601
                rating_time     = fields[3].replace('sec', '').replace(',', '.')
                filename        = fields[4]
                rating          = fields[-1].replace(',', '.') # always the last fields no matter what the flags are

                res_dict = OrderedDict()
                res_dict['playlist_id'] = playlist_id
                res_dict['log_id']      = log_id
                res_dict['timestamp']   = datetime_f
                res_dict['rating_time'] = rating_time
                res_dict['filename']    = filename
                res_dict['rating']      = rating

                data.append(res_dict)

            if state == "STATE_RATING_HISTOGRAM":
                break

        return(data)

if __name__ == '__main__':
    main()
