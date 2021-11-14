house=[1,2,3,4,5]
nationality=['brit', 'swede', 'dane', 'norwegian', 'german']
drink=['tea', 'coffee', 'milk', 'beer', 'water']
color=['red', 'white', 'green', 'yellow', 'blue']
smoke=['pall', 'dunhill', 'blend', 'bluemaster', 'prince']
pet=['dog', 'cat', 'horse', 'bird', 'fish']

secs = [nationality, drink, color, smoke, pet]

print("#Variables")
# print all variables
dic = {}
idx = 1
for i in range(len(secs)):
  for j in secs[i]:
      for h in house:
          var = j+'_in_house_'+str(h)
          dic[var] = idx
          print (idx, var);
          idx = idx + 1
# adding -
dic['-'] = '-'

  

          
print("#Now we define constraints")
for i in range(len(secs)):

  # exactly 1 house per j
  for j in secs[i]:
      big_or = ' v '.join([ j+'_in_house_'+str(h) for h in house ])
      _big_or = ' '.join([ str(dic[j+'_in_house_'+str(h)]) for h in house ])
      print(big_or)
      print(_big_or)
      for h1 in range(len(house)):
          for h2 in range(h1):
              small_or = '(-' + j+'_in_house_'+str(house[h1]) + ' v ' + '-' + j+'_in_house_'+str(house[h2]) +')'
              _small_or = '-' + str(dic[j+'_in_house_'+str(house[h1])]) + ' -' + str(dic[j+'_in_house_'+str(house[h2])])
              print(small_or)
              print(_small_or)
      

  # exactly 1 j per house
  for h in house:
      big_or = ' v '.join([ j+'_in_house_'+str(h) for j in secs[i] ])
      _big_or = ' '.join([ str(dic[j+'_in_house_'+str(h)]) for j in secs[i] ])
      print(big_or)
      print(_big_or)
      for j1 in range(len(secs[i])):
          for j2 in range(j1):
              small_or = '(-' + secs[i][j1]+'_in_house_'+str(h) + ' v ' + '-' + secs[i][j2]+'_in_house_'+str(h) +')'
              _small_or = '-' + str(dic[secs[i][j1]+'_in_house_'+str(h)]) + ' -' + str(dic[secs[i][j2]+'_in_house_'+str(h)])
              print(small_or)
              print(_small_or)
      

print("#brit lives in the red house")
n='brit'
c='red'
for h in house:
    m1 = n + '_in_house_' + str(h)
    m2 = c + '_in_house_' + str(h)
    small_or = '(- ' + m1 + ' v ' + m2 + ')'
    _small_or = '-' + str(dic[m1]) + ' ' + str(dic[m2])
    print(small_or)
    print(_small_or)

print("#swede keeps dogs as pets")
n='swede'
c='dog'
for h in house:
    m1 = n + '_in_house_' + str(h)
    m2 = c + '_in_house_' + str(h)
    small_or = '(- ' + m1 + ' v ' + m2 + ')'
    _small_or = '-' + str(dic[m1]) + ' ' + str(dic[m2])
    print(small_or)
    print(_small_or)


print("#dane drinks tea")
n='dane'
c='tea'
for h in house:
    m1 = n + '_in_house_' + str(h)
    m2 = c + '_in_house_' + str(h)
    small_or = '(- ' + m1 + ' v ' + m2 + ')'
    _small_or = '-' + str(dic[m1]) + ' ' + str(dic[m2])
    print(small_or)
    print(_small_or)

print("#green house is on left of white")
n='green'
c='white'
for h in house[:-1]:
    m1 = n + '_in_house_' + str(h)
    m2 = c + '_in_house_' + str(h+1)
    small_or = '(- ' + m1 + ' v ' + m2 + ')'
    _small_or = '-' + str(dic[m1]) + ' ' + str(dic[m2])
    print(small_or)
    print(_small_or)
print('- ' +n+'_in_house_' + str(house[-1]))
print('-' +str(dic[(n+'_in_house_' + str(house[-1]))]))

print("#green house owner drinks coffee")
n='green'
c='coffee'
for h in house:
    m1 = n + '_in_house_' + str(h)
    m2 = c + '_in_house_' + str(h)
    small_or = '(- ' + m1 + ' v ' + m2 + ')'
    _small_or = '-' + str(dic[m1]) + ' ' + str(dic[m2])
    print(small_or)
    print(_small_or)


print("#pall mall rears birds")
n='pall'
c='bird'
for h in house:
    m1 = n + '_in_house_' + str(h)
    m2 = c + '_in_house_' + str(h)
    small_or = '(- ' + m1 + ' v ' + m2 + ')'
    _small_or = '-' + str(dic[m1]) + ' ' + str(dic[m2])
    print(small_or)
    print(_small_or)

print("#yellow smokes dunhill")
n='yellow'
c='dunhill'
for h in house:
    m1 = n + '_in_house_' + str(h)
    m2 = c + '_in_house_' + str(h)
    small_or = '(- ' + m1 + ' v ' + m2 + ')'
    _small_or = '-' + str(dic[m1]) + ' ' + str(dic[m2])
    print(small_or)
    print(_small_or)

print("#center drinks milk")
print('milk_in_house_3')
print(str(dic['milk_in_house_3']))

print("#norwegian in first")
print('norwegian_in_house_1')
print(str(dic['norwegian_in_house_1']))

print("#one who smokes blends lives next to one who keeps cat")
n='blend'
c='cat'
for h in house[1:-1]:
    m1 = n + '_in_house_' + str(h)
    m2 = c + '_in_house_' + str(h+1)
    m3 = c + '_in_house_' + str(h-1)
    small_or = '(- ' + m1 + ' v ' + m2 + ' v ' + m3 +  ')'
    _small_or = '-' + str(dic[m1]) + ' ' + str(dic[m2]) + ' ' + str(dic[m3])
    print(small_or)
    print(_small_or)

h = house[0]
m1 = n + '_in_house_' + str(h)
m2 = c + '_in_house_' + str(h+1)
small_or = '(- ' + m1 + ' v ' + m2 + ')'
_small_or = '-' + str(dic[m1]) + ' ' + str(dic[m2])
print(small_or)
print(_small_or)

h = house[-1]
m1 = n + '_in_house_' + str(h)
m2 = c + '_in_house_' + str(h-1)
small_or = '(- ' + m1 + ' v ' + m2 + ')'
_small_or = '-' + str(dic[m1]) + ' ' + str(dic[m2])
print(small_or)
print(_small_or)


print("#one who keeps horse lives next to dunhill smoker")
n='horse'
c='dunhill'
for h in house[1:-1]:
    m1 = n + '_in_house_' + str(h)
    m2 = c + '_in_house_' + str(h+1)
    m3 = c + '_in_house_' + str(h-1)
    small_or = '(- ' + m1 + ' v ' + m2 + ' v ' + m3 +  ')'
    _small_or = '-' + str(dic[m1]) + ' ' + str(dic[m2]) + ' ' + str(dic[m3])
    print(small_or)
    print(_small_or)

h = house[0]
m1 = n + '_in_house_' + str(h)
m2 = c + '_in_house_' + str(h+1)
_small_or = '-' + str(dic[m1]) + ' ' + str(dic[m2])
small_or = '(- ' + m1 + ' v ' + m2 + ')'
print(small_or)
print(_small_or)


h = house[-1]
m1 = n + '_in_house_' + str(h)
m2 = c + '_in_house_' + str(h-1)
small_or = '(- ' + m1 + ' v ' + m2 + ')'
_small_or = '-' + str(dic[m1]) + ' ' + str(dic[m2])
print(small_or)
print(_small_or)

print("#who smokes bluemaster drinks beer")
n='bluemaster'
c='beer'
for h in house:
    m1 = n + '_in_house_' + str(h)
    m2 = c + '_in_house_' + str(h)
    small_or = '(- ' + m1 + ' v ' + m2 + ')'
    _small_or = '-' + str(dic[m1]) + ' ' + str(dic[m2])
    print(small_or)
    print(_small_or)


print("#german smokes prince")
n='german'
c='prince'
for h in house:
    m1 = n + '_in_house_' + str(h)
    m2 = c + '_in_house_' + str(h)
    small_or = '(- ' + m1 + ' v ' + m2 + ')'
    _small_or = '-' + str(dic[m1]) + ' ' + str(dic[m2])
    print(small_or)
    print(_small_or)


print("#norwegian lives next to blue house")
n='norwegian'
c='blue'
for h in house[1:-1]:
    m1 = n + '_in_house_' + str(h)
    m2 = c + '_in_house_' + str(h+1)
    m3 = c + '_in_house_' + str(h-1)
    small_or = '(- ' + m1 + ' v ' + m2 + ' v ' + m3 +  ')'
    print(small_or)
    print(_small_or)

h = house[0]
m1 = n + '_in_house_' + str(h)
m2 = c + '_in_house_' + str(h+1)
small_or = '(- ' + m1 + ' v ' + m2 + ')'
_small_or = '-' + str(dic[m1]) + ' ' + str(dic[m2])
print(small_or)
print(_small_or)

h = house[-1]
m1 = n + '_in_house_' + str(h)
m2 = c + '_in_house_' + str(h-1)
small_or = '(- ' + m1 + ' v ' + m2 + ')'
_small_or = '-' + str(dic[m1]) + ' ' + str(dic[m2])
print(small_or)
print(_small_or)


print("#one who smoeks blend is next to water drinker")
n='blend'
c='water'
for h in house[1:-1]:
    m1 = n + '_in_house_' + str(h)
    m2 = c + '_in_house_' + str(h+1)
    m3 = c + '_in_house_' + str(h-1)
    small_or = '(- ' + m1 + ' v ' + m2 + ' v ' + m3 +  ')'
    _small_or = '-' + str(dic[m1]) + ' ' + str(dic[m2]) + ' ' + str(dic[m3])
    print(small_or)
    print(_small_or)

h = house[0]
m1 = n + '_in_house_' + str(h)
m2 = c + '_in_house_' + str(h+1)
small_or = '(- ' + m1 + ' v ' + m2 + ')'
_small_or = '-' + str(dic[m1]) + ' ' + str(dic[m2])
print(small_or)
print(_small_or)

h = house[-1]
m1 = n + '_in_house_' + str(h)
m2 = c + '_in_house_' + str(h-1)
small_or = '(- ' + m1 + ' v ' + m2 + ')'
_small_or = '-' + str(dic[m1]) + ' ' + str(dic[m2])
print(small_or)
print(_small_or)

