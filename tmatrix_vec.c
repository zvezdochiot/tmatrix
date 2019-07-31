/**
 * @file tmatrix_vec.c
 * @author Stanislav Mikhel
 * @date 2019
 * @brief Vector specific operations.
 */ 
#include <stdlib.h>
#include <math.h>
#include "tmatrix_vec.h"
#include "tmatrix_priv.h"

tmSize vec_len(tMat *m)
{
  if(m) {
    if(m->cols == 1) {
      return m->rows;
    } else if(m->rows == 1) {
      return m->cols;
    }
  }
  return 0;
}

tmVal vec_get(tMat *m, tmSize k, int *err)
{
  int e = 0;
  tmVal res = 0;

  if(m) {
    if(m->cols == 1) {
      /* column vector */
      if(k < m->rows)
        res = *tm_at(m,k,0);
      else 
        e = TM_ERR_WRONG_SIZE;      
    } else if(m->rows == 1) {
      /* row vector */
      if(k < m->cols)
        res = *tm_at(m,0,k);
      else 
        e = TM_ERR_WRONG_SIZE;      
    } else 
      e = TM_ERR_NOT_VEC;
  } else 
    e = TM_ERR_EMPTY_ARGS;
    
  if(err) *err = e;
  
  return res;
}

void vec_set(tMat *m, tmSize k, tmVal v, int *err)
{
  int e = 0;
  
  if(m) {
    if(m->cols == 1) {
      /* column vector */      
      if(k < m->rows)
        *tm_at(m,k,0) = v;
      else 
        e = TM_ERR_WRONG_SIZE; 
    } else if(m->rows == 1) {
      /* row vector */      
      if(k < m->cols)
        *tm_at(m,0,k) = v;
      else 
        e = TM_ERR_WRONG_SIZE;          
    } else 
      e = TM_ERR_NOT_VEC;
  } else 
    e = TM_ERR_EMPTY_ARGS;
    
  if(err) *err = e;
}

tmVal vec_dot(tMat *a, tMat *b, int *err)
{
  int e = 0,i,N1;
  tmVal sum = 0;
  
  N1 = vec_len(a); i = vec_len(b);
  if(N1 && i) {
    if(N1 == i) {
      if(IS_PRIM(a) && IS_PRIM(b)) {
        for(i = 0; i < N1; i++) 
          sum += a->data[i] * b->data[i];        
      } else {
        for(i = 0; i < N1 && !e; i++)
          sum += vec_get(a,i,&e) * vec_get(b,i,&e);
      }
    } else 
      e = TM_ERR_NOT_COMPAT;
  } else 
    e = TM_ERR_NOT_VEC;
  
  if(err) *err = e;
  
  return sum;
}

int vec_cross(tMat *res, tMat *a, tMat *b, int *err)
{
  int e = 0;
  tmVal *data = 0, a0,a1,a2,b0,b1,b2;
  
  
  if(res && a && b) {
    if(vec_len(a) == 3 && vec_len(b) == 3) {
      if(vec_len(res) != 3) {
        if(res->type == TM_MAIN) {
          if(res->rows * res->cols < 3) {            
            data = (tmVal*) malloc(3*sizeof(tmVal));
            if(data) {
              free(res->data);
              res->data = data;
            } else 
              e = TM_ERR_NO_MEMORY;            
            if(!e) {
              res->rows = 3;
              res->cols = 1;
              res->width = 1;
            } else {
              if(err) *err = e;
              return !e;
            }
          }          
        } else 
          e = TM_ERR_NOT_MAIN; 
      }
      a0 = vec_get(a,0,0); a1 = vec_get(a,1,0); a2 = vec_get(a,2,0);
      b0 = vec_get(b,0,0); b1 = vec_get(b,1,0); b2 = vec_get(b,2,0);
      if(!e) vec_set(res,0,a1*b2-b1*a2,&e);
      if(!e) vec_set(res,1,a2*b0-a0*b2,&e);
      if(!e) vec_set(res,2,a0*b1-b0*a1,&e);      
    } else
      e = TM_ERR_NOT_DEF;
  } else 
    e = TM_ERR_EMPTY_ARGS;
    
  if(err) *err = e;
  
  return !e;  
}

tmVal vec_norm2(tMat *m, int *err)
{
  int e = 0, i;
  tmVal sum = 0, v;
  
  if(m) {
    if(m->cols == 1) {
      /* column vector */
      for(i = 0; i < m->rows; i++) {
        v = *tm_at(m,i,0);
        sum += v * v;
      }
    } else if(m->rows == 1) {
      /* row vector */
      for(i = 0; i < m->cols; i++) {
        v = *tm_at(m,0,i);
        sum += v * v;
      }
    } else 
      e = TM_ERR_NOT_VEC;
  } else 
    e = TM_ERR_EMPTY_ARGS;
    
  if(err) *err = e;
  
  return sum;
}

int vec_normalize(tMat *m, int *err)
{
  int e = 0, i;
  tmVal sum = 0, v;
  
  if(m) {    
    if(m->cols == 1) {
      /* column vector */
      for(i = 0; i < m->rows; i++) {
        v = *tm_at(m,i,0);
        sum += v * v;
      }      
      if(sum > 0) {
        sum = sqrt(sum);
        for(i = 0; i < m->rows; i++) 
          *tm_at(m,i,0) /= sum;
      }
    } else if(m->rows == 1) {
      /* row vector */
      for(i = 0; i < m->cols; i++) {
        v = *tm_at(m,0,i);
        sum += v * v;
      }      
      if(sum > 0) {
        sum = sqrt(sum);
        for(i = 0; i < m->cols; i++) 
          *tm_at(m,0,i) /= sum;
      }
    } else 
      e = TM_ERR_NOT_VEC;    
  } else 
    e = TM_ERR_EMPTY_ARGS;
    
  if(err) *err = e;
  
  return !e;
}

