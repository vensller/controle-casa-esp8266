import styled from 'styled-components/native';

export const Container = styled.ScrollView`
  flex: 1;
`;

export const Title = styled.Text`
  font-size: 20px;
  color: #fff;
  font-weight: bold;
  align-self: center;
  margin-top: 30px;
`;

export const Button = styled.TouchableOpacity`
  margin: 10px;
  padding: 10px;
  border: 0;
  background: #2d2f5e;
  border-radius: 4px;
`;

export const Text = styled.Text`
  color: #fff;
  font-weight: bold;
  font-size: 16px;
`;

export const ButtonsContainer = styled.View`
  margin-top: 30px;
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
`;