
#include <knotkit.h>

// test for ring

template<class R> void
test_ring (int p)
{
  R zero (0);
  R one (1);
  R minus_one (-1);
  
  assert (zero == 0);
  assert (zero | zero);
  assert (one | zero);
  assert (minus_one | zero);
  assert (! (zero | one));
  assert (! (zero | minus_one));
  
  assert (one.is_unit ());
  assert (minus_one.is_unit ());
  assert (one.recip () == one);
  assert (minus_one.recip () == minus_one);
  
  if (p)
    assert (R (p) == 0);
  
  if (p != 2)
    assert (one != minus_one);
  
  int n = (p
	   ? std::min (p, 20)
	   : 20);
  for (int i = -n; i <= n; i ++)
    {
      R x (i);
      if (x.is_unit ())
	{
	  assert (x * x.recip () == one);
	  assert (x.recip () * x == one);
	  assert (x.recip ().recip () == x);
	}
      
      assert (one | x);
      assert (minus_one | x);
      
      if (x != 0)
	{
	  assert (x | zero);
	  assert (! (zero | x));
	}
      
      for (int j = -n; j <= n; j ++)
	{
	  R y (j);
	  
	  assert (- (-x) == x);
	  assert (x + y == y + x);
	  assert (x * y == y * x);
	  
	  if (x != 0 && x | y)
	    {
	      R q = y.div (x);
	      assert (y == q * x);
	    }
	  
	  if (x != 0 || y != 0)
	    {
	      triple<R, R, R> t = x.extended_gcd (y);
	      assert (t.first == t.second*x + t.third*y);
	    }
	  
	  for (int k = -n; k <= n; k ++)
	    {
	      R z (k);
	      
	      assert ((x + y) + z == x + (y + z));
	      assert ((x * y) * z == x * (y * z));
	      assert (x*(y + z) == x*y + x*z);
	      assert ((x + y)*z == x*z + y*z);
	    }
	}
    }
}

template<class F> void
test_field ()
{
  for (unsigned i = 1; i <= 8; i ++)
    for (unsigned j = 1; j <= rolfsen_crossing_knots (i); j ++)
      {
	knot_diagram kd (rolfsen_knot (i, j));
	
	show (kd); newline ();
	
	cube<F> c (kd);
	mod_map<F> d = c.compute_d (1, 0, 0, 0, 0);
	
	assert (d.compose (d) == 0);
	
	ptr<const quotient_module<F> > H = d.homology ();
	display ("H:\n", *H);
	
	chain_complex_simplifier<F> s (c.khC, d, 1);
	display ("s.new_C:\n", *s.new_C);
	
	assert (H->dim () == s.new_C->dim ());
      }
}

int
main ()
{
#if 0
  knot_diagram kd (rolfsen_knot (8, 19));
  cube<Z2> c (kd);
  sseq ss = compute_szabo_sseq (c);
  multivariate_laurentpoly<Z> ssp = ss.pages[1].poincare_polynomial (ss.bounds);
  
  display ("ssp: ", ssp);
  
  multivariate_laurentpoly<Z> p;
  p.muladdeq (5, multivariate_laurent_monomial (VARIABLE, 1, -2));
  p.muladdeq (-6, multivariate_laurent_monomial (VARIABLE, 2, 13));
  p.muladdeq (14, (multivariate_laurent_monomial (VARIABLE, 1, 5)
		   * multivariate_laurent_monomial (VARIABLE, 2, -6)));
  display ("p: ", p);
  
  display ("p*p: ", p*p);

  {
    writer w ("dump");
    write (w, p*p);
  }

  {
    reader r ("dump");
    multivariate_laurentpoly<Z> q (r);
    
    display ("q: ", q);
    
    assert (q == p*p);
  }
#endif
  
  multivariate_laurentpoly<Z> p = -11;
  p.muladdeq (5, VARIABLE, 1);
  p.muladdeq (7, VARIABLE, 2);
  p.muladdeq (-3, VARIABLE, 3);

  multivariate_laurentpoly<Z> q = p*p + p + 23;
  multivariate_laurentpoly<Z> r = q*q - Z (7)*p + 81;
  
  multivariate_laurentpoly<Z> s = r - p*q + 10;

  display ("p:", p);
  display ("q:", q);
  display ("r:", r);
  display ("s:", s);
  
  map<multivariate_laurentpoly<Z>, std::string> m;
  m.push (p, "p");
  m.push (q, "q");
  m.push (r, "thisisr");
  
  assert (m % p);
  assert (m % q);
  assert (m % r);
  assert (! (m % s));
  
  assert (m(p) == "p");
  assert (m(q) == "q");
  assert (m(r) == "thisisr");
  
  std::string str ("This is a test.");
  
  {
    writer w ("test.dat");
    write (w, m);
  }
  
  reader rdr ("test.dat");
  map<multivariate_laurentpoly<Z>, std::string> m2 (rdr);
    
  assert (m == m2);
    
  assert (m2(p) == "p");
  assert (m2(q) == "q");
  assert (m2(r) == "thisisr");
  
#if 0
  test_ring<Z2> (2);
  test_ring<Z> (0);
  test_ring<Q> (0);
  test_ring<Zp<2> > (2);
  test_ring<Zp<3> > (3);
  test_ring<Zp<5> > (5);
  test_ring<Zp<7> > (7);
  
  test_field<Q> ();
  test_field<Zp<7> > ();
  test_field<Zp<5> > ();
  test_field<Zp<3> > ();
  test_field<Z2> ();
  test_field<Zp<2> > ();
#endif  
}
