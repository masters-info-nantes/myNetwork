# myNetwork

- usage of [ColorLog](https://github.com/kuroidoruido/ColorLog)

### protocole messages

- newIdC : unique id generate by server for one client
- idC : id of the client
- idSC : the id of the client which have ask the task

#### new client connexion
- request
```
CLIENT
ON
```
- answer
```
OK newIdC
```

#### client deconnexion
- request
```
CLIENT idC
OFF
```
- answer
```
OK
```

#### client reserve another client for a task
```
MASTER idSC
TRY
```
- answer
```
OK idC
```
or
```
KO
```

#### client assign a task to a client
```
MASTER idSC
ASK idC
```
- answer
```
OK idC
```
or
```
KO idC
```

#### client response for the task
```
MASTER idSC
RES idC
data
```
- answer
```
OK
```
