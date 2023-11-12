s='''d e c r y p t _ p a c k e t ( g n u t l s _ s e s s i o n _ t s e s s i o n , g n u t l s _datum_ t ∗ c i p h e r t e x t ,
g n u t l s _datum_ t ∗ p l ai n , c o n t e n t _ t y p e_ t type , r e c o r d _ p a r am e t e r s _ s t ∗ params ,
g n u t l s _ u i n t 6 4 ∗ s e q u e n c e ) {
. . .
pad = p l ai n −>da ta [ c i p h e r t e x t −> s i z e − 1 ] ; / ∗ pad ∗ /
. . .
fo r ( i = 2 ; i <= MIN( 2 5 6 , c i p h e r t e x t −> s i z e ) ; i ++) {
tmp_pad_ f a i l e d | = ( p l ai n −>da ta [ c i p h e r t e x t −> s i z e − i ] ! = pad ) ;
pad_ f a i l e d | = ( ( i <= ( 1 + pad ) ) & ( tmp_pad_ f a i l e d ) ) ;
}
i f ( u n l i k e l y ( pad_ f a i l e d ! = 0 | | ( 1 + pad > ( ( i n t ) c i p h e r t e x t −> s i z e − t a g _ s i z e ) ) ) ) {
/ ∗ We do n o t f a i l h e r e . We c h e c k b el ow f o r t h e
∗ t h e pad_ f a i l e d . I f z e r o means s u c c e s s .
∗ /
pad_ f a i l e d = 1 ;
pad = 0 ;
}
leng th = c i p h e r t e x t −> s i z e − t a g _ s i z e − pad − 1 ;
. . .
r e t = _ g n u t l s _a u t h_ c i p h e r _add_a u t h (& params−> r e a d . c t x . t l s 1 2 , p l ai n −>data , leng th ) ;
i f ( u n l i k e l y ( g n u t l s _memcmp( ta g , t a g _ p t r , t a g _ s i z e ) ! = 0 | | pad_ f a i l e d ! = 0 ) ) {
/ ∗ HMAC was n o t t h e same . ∗ /
dummy_w ai t ( params , p l ai n , pad_ f a i l e d , pad , leng th + p r eambl e_ s i z e ) ;
'''

print(s.replace(' ', ''))