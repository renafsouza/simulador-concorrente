# simulador-concorrente

Simulador de atendimento bancario que explora programação concorrente. Trabalho da disciplina de Sistemas Operacionais.

## Como executar:

### Com parâmetros padrão, executar:

```
make
```

### Com parâmetros personalizados, executar:

```
make build
./simulador.out $1 $2 $3 $4 $5 $6 $7
```

#### Onde:

$1 = tick

$2 = n_seats

$3 = n_columns

$4 = client_interval_min

$5 = client_interval_max

$6 = atendimento_interval_min

$7 = atendimento_interval_max

### Exemplo:

```
make build
./simulador.out 1 9 3 1 14 5 11
```
