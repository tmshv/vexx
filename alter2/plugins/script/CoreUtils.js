function assert(exp)
  {
  if(!exp)
    {
    script.enableDebugging(true);

    throw arguments.length > 1 ? arguments[1] : "Assertion failed";
    return false;
    }
  return true;
  }
