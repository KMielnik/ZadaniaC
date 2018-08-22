import omdb
from datetime import datetime
import os
import mimetypes
import re

API_KEY = "4ed3fc46"
omdb.set_default('apikey', API_KEY)

running = True


def downloadTitlesFromFolder(folder: str):
    unwantedWords = ['5.1', '7.1', '5 1', '7 1', 'DUAL AUDIO', 'DUAL-AUDIO', 'MULTI-CHANNEL', 'Ita-Eng',
                     '2160p', '4K', '1080p', '720p', '480p', '360p', 'HD', 'FULL HD', 'FULLHD',
                     'x264', 'CH', 'X264', 'HEVC', 'WEB-DL', 'BrRip', 'Rip', 'DVDRip', 'DVD', 'XviD', 'BLURAY',
                     'EXTENDED', 'REMASTERED', 'DIRECTORS', 'UNRATED', 'AlTERNATE']
    movieExtensions = ['.mkv']
    for ext in mimetypes.types_map:
        if mimetypes.types_map[ext].split('/')[0] == 'video':
            movieExtensions.append(ext)

    titles = ''
    print(movieExtensions)

    for file in os.listdir(folder):
        fileExtension = re.findall(r'[.].{2,4}', file)[-1]
        if fileExtension in movieExtensions:
            title = file.replace(fileExtension, '')
            for word in unwantedWords:
                title = title.replace(word, '')
            title = title.lstrip().rstrip()
            titles += f'{title}, '
    return titles[:-2]


while running:
    movies = []
    movieFolder = 'C:\Movies'
    userInput = input(f"[W]pisujesz nazwy, czy mam je [P]obrać z '{movieFolder}'? : ")
    if userInput == 'W':
        userInput = input("Podaj tytuły filmów: ")
    elif userInput == 'P':
        userInput = downloadTitlesFromFolder(movieFolder)
    else:
        continue

    titleNames = userInput.split(' : ')[0]
    try:
        sortBy = userInput.split(' : ')[1]
    except:
        sortBy = ''

    for title in titleNames.split(', '):
        movie = omdb.get(title=title)
        if len(movie) == 0:
            print(f"Nie znaleziono {title} w bazie.")
        else:
            movies.append(movie)

    if sortBy == 'Rating':
        print(f"Sorting by {sortBy} from best rated")
        movies.sort(key=lambda x: (int(x['ratings'][1]['value'].replace('%', '')), x), reverse=False)

    if sortBy == 'Length':
        print(f"Sorting by {sortBy} from longest")
        movies.sort(key=lambda x: (int(x['runtime'].replace(' min', '')), x), reverse=True)

    if sortBy == 'Date':
        print(f"Sorting by {sortBy} from newest")
        movies.sort(key=lambda x: (datetime.strptime(x['released'], '%d %b %Y'), x), reverse=True)

    if sortBy == 'Popularity':
        print(f"Sorting by {sortBy} from most voted")
        movies.sort(key=lambda x: (int(x['imdb_votes'].replace(',', '')), x), reverse=True)

    for movie in movies:
        print(f"Title: {movie['title']}")
        print(f"\tRelease date: {movie['released']}")
        movieDatetime = datetime.strptime(movie['released'], '%d %b %Y')
        print(f"\tRelease date: {movieDatetime:%d.%m.%Y}")
        print(f"\tRatings:")
        for rating in movie['ratings']:
            print(f"\t\t{rating['source']}: {rating['value']}")
        print(f"\tPopularity(votes): {movie['imdb_votes']}")
        print(f"\tLength: {movie['runtime']}")
        print()

    userInput = input("Chcesz kontynuować? ('end' konczy program): ")
    if userInput == 'end':
        running = False
    print()
