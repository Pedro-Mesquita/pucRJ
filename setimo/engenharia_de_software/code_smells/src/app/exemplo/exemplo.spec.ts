import { ComponentFixture, TestBed } from '@angular/core/testing';

import { Exemplo } from './exemplo';

describe('Exemplo', () => {
  let component: Exemplo;
  let fixture: ComponentFixture<Exemplo>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      imports: [Exemplo],
    }).compileComponents();

    fixture = TestBed.createComponent(Exemplo);
    component = fixture.componentInstance;
    await fixture.whenStable();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
