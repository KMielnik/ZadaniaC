import omdb
from datetime import datetime
import os
import mimetypes
import re
import urllib.request
import urllib.parse
import webbrowser


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
        if not os.path.isfile(os.path.join(folder, file)):
            continue
        fileExtension = re.findall(r'[.].{2,4}', file)[-1]
        if fileExtension in movieExtensions:
            title = file.replace(fileExtension, '')
            for word in unwantedWords:
                title = title.replace(word, '')
            title = title.lstrip().rstrip()
            titles += f'{title}, '
    return titles[:-2]


def printMoviesToConsole(movies):
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


def createFoldersForMovies(movies, moviesFolder):
    for movie in movies:
        if not os.path.exists(f"{moviesFolder}/{movie['title']}"):
            os.mkdir(f"{moviesFolder}/{movie['title']}")
        movie_description = open(f"{moviesFolder}/{movie['title']}/opis.txt", 'w')
        movie_description.write(str(movie))
        movie_description.close()
        urllib.request.urlretrieve(movie['poster'], f"{moviesFolder}/{movie['title']}/poster.jpg")


def sortMovies(movies, sortBy):
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
    return movies


def createMoviesDatabase(userInput):
    movies = []
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
    return movies, sortBy


def openTrailers(movies):
    for movie in movies:
        query_string = urllib.parse.urlencode({"search_query": movie['title'] + ' trailer'})
        html_content = urllib.request.urlopen("http://www.youtube.com/results?" + query_string)
        search_results = re.findall(r'href=\"\/watch\?v=(.{11})', html_content.read().decode())
        webbrowser.open("http://www.youtube.com/watch?v=" + search_results[0])


def main():
    omdb.set_default('apikey', '4ed3fc46')

    while True:
        moviesFolder = 'C:\Movies'
        userInput = input(f"[W]pisujesz nazwy, czy mam je [P]obrać z '{moviesFolder}'? : ")
        if userInput == 'W':
            userInput = input("Podaj tytuły filmów: ")
        elif userInput == 'P':
            userInput = downloadTitlesFromFolder(moviesFolder)
        else:
            continue

        movies, sortBy = createMoviesDatabase(userInput)
        sortMovies(movies, sortBy)
        printMoviesToConsole(movies)
        createFoldersForMovies(movies, moviesFolder)

        userInput = input("Chcesz wyświetlić trailery? [T]ak : ")
        if userInput == 'T':
            openTrailers(movies)

        userInput = input("Chcesz kontynuować? ('end' konczy program): ")
        if userInput == 'end':
            break
        print()


if __name__ == "__main__":
    main()
