import omdb
from datetime import datetime
API_KEY = "4ed3fc46"
omdb.set_default('apikey', API_KEY)

running = True

while running:
    movies = []
    userInput = input("Podaj tytuły filmów: ")
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
        movies.sort(key=lambda x: (datetime.strptime(x['released'],'%d %b %Y'), x), reverse=True)

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
              
