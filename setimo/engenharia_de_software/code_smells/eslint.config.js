const eslint = require('@eslint/js');
const { defineConfig } = require('eslint/config');

const tseslint = require('typescript-eslint');
const angular = require('angular-eslint');

module.exports = defineConfig([
  {
    files: ['**/*.ts'],

    extends: [
      // Regras recomendadas do ESLint para JavaScript em geral.
      eslint.configs.recommended,

      // Regras recomendadas do TypeScript ESLint.
      ...tseslint.configs.recommended,

      // Regras de estilo do TypeScript ESLint.
      ...tseslint.configs.stylistic,

      // Regras recomendadas para Angular em arquivos .ts.
      ...angular.configs.tsRecommended,
    ],
    processor: angular.processInlineTemplates,

    rules: {
      '@angular-eslint/component-selector': [
        'error',
        {
          type: 'element',
          prefix: 'app',
          style: 'kebab-case',
        },
      ],

      'max-lines-per-function': [
        'error',
        {
          max: 20,
          skipBlankLines: true,
          skipComments: true,
          IIFEs: true,
        },
      ],

      '@typescript-eslint/explicit-function-return-type': 'error',


      '@typescript-eslint/no-unused-vars': 'error',

      '@typescript-eslint/no-explicit-any': 'error',

      'no-console': 'error',

      'eqeqeq': ['error', 'always'],

      'max-params': ['error', 4],

      'complexity': ['error', 5],
    },
  },

  {
    files: ['**/*.html'],
    extends: [
      ...angular.configs.templateRecommended,
      ...angular.configs.templateAccessibility,
    ],
    rules: {},
  },
]);