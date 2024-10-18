-- 1
with cerveja_padoca as (
    select
        cerveja
    from
        venda
    where
        bar = 'Padoca'
)
select
    distinct pessoa
from
    gostam,
    cerveja_padoca
where
    gostam.cerveja = cerveja_padoca.cerveja 
    
    
-- 2
(
    select 
        cerveja
    from 
        venda
)
