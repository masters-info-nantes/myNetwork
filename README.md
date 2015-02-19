# myNetwork

- usage of [ColorLog](https://github.com/kuroidoruido/ColorLog)

### protocole messages

- newIdC : unique id generate by server for one client
- idC : id of the client
- idSC : the id of the client which have ask the task
- data : could be on more than one line

#### unrecognize request
- answer
```
KO <error>
```

#### new client connection
- request
```
CLIENT
ON
```
or
```
MASTER
ON
```
Note: with MASTER indication, client is register with a working status
- answer
```
OK newIdC
```

#### client disconnection
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
- request
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
KO <error>
```

#### client assign a task to a client
- request
```
MASTER idSC
ASK idC
data
```
- answer
```
OK idC
```
or
```
KO <error>
```

#### client response for the task
- request
```
MASTER idSC
RES idC
data
```
- answer
```
OK idSC
```
or
```
KO <error>
```

#### client ask for waiting request
- request
```
CLIENT idC
WAITING
```
- answer
the request that waiting forward (same as "client assign a task to a client" request)
or
```
NOTHING
```
