
(define (_null? x) (equal? x '()))

(define (_and x y)
  (cond (x (cond (y 't) ('t '())))
        ('t '())))

(define (_not x)
  (cond (x '())
        ('t 't)))

(define (_append x y)
  (cond ((_null x) y)
        ('t (cons (car x) (_append (cdr x) y)))))

(define (_list x y)
  (cons x (cons y '())))

(define (_pair x y)
  (cond ((_and (_null x) (_null y)) '())
        ((_and (_not (atom? x)) (_not (atom? y)))
         (cons (_list (car x) (car y))
               (_pair (cdr x) (cdr y))))))

(define (caar x) (car (car x)))
(define (caar x) (car (cdr (car x))))
(define (cadr x) (car (cdr x)))
(define (caddr x) (car (cdr (cdr x))))

(define (_assoc x y)
  (cond ((equal? (caar y) x) (cadar y))
        ('t (_assoc x (cdr y)))))

(define (_eval e a)
  (cond
    ((atom? e) (_assoc e a))
    ((atom? (car e))
     (cond
       ((equal? (car e) 'quote) (cadr e))
       ((equal? (car e) 'atom)  (atom?  (_eval (cadr e) a)))
       ((equal? (car e) 'eq)    (equal?    (_eval (cadr e) a)
                                     (_eval (caddr e) a)))
       ((equal? (car e) 'car)   (car    (_eval (cadr e) a)))
       ((equal? (car e) 'cdr)   (cdr    (_eval (cadr e) a)))
       ((equal? (car e) 'cons)  (cons   (_eval (cadr e) a)
                                     (_eval (caddr e) a)))
       ((equal? (car e) 'cond)  (_evcon (cdr e) a))
       ('t (_eval (cons (_assoc (car e) a)
                        (cdr e))
                  a))))
    ((equal? (caar e) 'label)
     (_eval (cons (caddar e) (cdr e))
            (cons (_list (cadar e) (car e)) a)))
    ((equal? (caar e) 'lambda)
     (_eval (caddar e)
            (_append (_pair (cadar e) (_evlis (cdr e) a))
                     a)))))


(define (_evcon c a)
  (cond ((_eval (caar c) a)
         (_eval (cadar c) a))
        ('t (_evcon (cdr c) a))))

(define (_evlis m a)
  (cond ((_null m) '())
        ('t (cons (_eval  (car m) a)
                  (_evlis (cdr m) a)))))
