void converte (float val, double * novo_val) {
    union Uniao {
        float f;
        double d;
    }

    Uniao conv;
    conv.f = val
    *novo.val = conv.d
}