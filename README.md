# proj_micro

Projeto da disciplina de projeto de sistemas microcontrolados. O programa em questão é um multímetro ao qual realiza a leitura de medidas de indutância, capacitância, resistência, corrente, tensão ajustando automaticamente a escala das grandezas. O controle é realizado por um microcontrolador ATmega328P (Arduino UNO).

- `if (modo == 0)` Leitura de tensão;
- `if (modo == 1)` Leitura de resistência;
- `if (modo == 2)` Leitura de capacitância;
- `if (modo == 3)` Leitura de corrente;
- `if (modo == 4)` Leitura de indutância.
