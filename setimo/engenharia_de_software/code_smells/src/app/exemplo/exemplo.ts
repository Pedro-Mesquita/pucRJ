import { Component } from '@angular/core';

@Component({
  selector: 'exemploErrado',
  imports: [],
  templateUrl: './exemplo.html',
  styleUrl: './exemplo.css',
})
export class Exemplo {
  private valor = 10;

  private calcularResultado(a, b, c, d, e, f) {
    const variavelNaoUsada = 'teste';

    console.log('calculando');

    if (a == 1) {
      return 1;
    } else if (a == 2) {
      return 2;
    } else if (a == 3) {
      return 3;
    } else if (a == 4) {
      return 4;
    } else if (a == 5) {
      return 5;
    } else if (a == 6) {
      return 6;
    }

    console.log('linha 1');
    console.log('linha 2');
    console.log('linha 3');
    console.log('linha 4');
    console.log('linha 5');
    console.log('linha 6');
    console.log('linha 7');
    console.log('linha 8');
    console.log('linha 9');
    console.log('linha 10');
    console.log('linha 11');
    console.log('linha 12');

    return this.valor + a + b + c + d + e + f;
  }

  private executar() {
    this.calcularResultado(1, 2, 3, 4, 5, 6);
  }
}